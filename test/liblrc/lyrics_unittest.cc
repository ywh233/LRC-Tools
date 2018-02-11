//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 09 2018
//  File : lyrics_unittest.cc
//******************************************

#include "liblrc/lyrics.h"

#include <array>
#include <memory>
#include <tuple>

#include "gtest/gtest.h"

namespace lrc {

class LyricsTest : public testing::Test {
 protected:
  std::unique_ptr<Lyrics> CreateTestLyrics(int32_t adjustment);
  void VerifyLyricAtOffset(Lyrics* lyrics,
                           uint32_t offset,
                           uint32_t end_time,
                           const std::string& line);
};

std::unique_ptr<Lyrics> LyricsTest::CreateTestLyrics(int32_t adjustment) {
  std::vector<Lyrics::LyricLine> test_lines = {
      {100, 0, "Line:100"},
      {70, 0, "Line:70"},
      {60, 0, "Line:60"},
      {40, 0, "Line:40"},
      {20, 0, "Line:20"},
      {40, 0, "Line:40"},
  };
  Lyrics::Metadata metadata {
    "artist",
    "album",
    "title",
    "lyricsWriter",
    "fileCreator",
    adjustment
  };
  return std::make_unique<Lyrics>(metadata, std::move(test_lines));
}


void LyricsTest::VerifyLyricAtOffset(Lyrics* lyrics,
                                     uint32_t offset,
                                     uint32_t end_time,
                                     const std::string& line) {
  EXPECT_EQ(line, lyrics->LyricAt(offset).lyric);
  EXPECT_EQ(end_time, lyrics->LyricAt(offset).end_time);
}

// Test cases

TEST_F(LyricsTest, VerifyMetadata) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(-1);

  EXPECT_EQ("artist", lyrics->GetMetadata().artist);
  EXPECT_EQ("album", lyrics->GetMetadata().album);
  EXPECT_EQ("title", lyrics->GetMetadata().title);
  EXPECT_EQ("lyricsWriter", lyrics->GetMetadata().lyricsWriter);
  EXPECT_EQ("fileCreator", lyrics->GetMetadata().fileCreator);
  EXPECT_EQ(-1, lyrics->GetMetadata().adjustment);
}

TEST_F(LyricsTest, LookupEmptyLyrics) {
  Lyrics::Metadata meta;
  meta.adjustment = 0;
  Lyrics lyrics(meta, {});

  EXPECT_EQ("", lyrics.LyricAt(1).lyric);
  EXPECT_EQ(lyrics.IteratorBegin(), lyrics.LyricIteratorAt(1));
}

TEST_F(LyricsTest, LookupFirstLyric) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  EXPECT_EQ(lyrics->IteratorBegin(), lyrics->LyricIteratorAt(0));
  VerifyLyricAtOffset(lyrics.get(), 0, 20, "");
}

TEST_F(LyricsTest, LookupInTheMiddle) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  VerifyLyricAtOffset(lyrics.get(), 50, 60, "Line:40");
}

TEST_F(LyricsTest, LookupAtTheEnd) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  EXPECT_EQ(lyrics->IteratorEnd() - 1, lyrics->LyricIteratorAt(200));
  VerifyLyricAtOffset(lyrics.get(), 200, Lyrics::kEndTimeNever, "Line:100");
}

TEST_F(LyricsTest, LookupTwiceWithinDuration) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  VerifyLyricAtOffset(lyrics.get(), 80, 100, "Line:70");
  VerifyLyricAtOffset(lyrics.get(), 90, 100, "Line:70");
}

TEST_F(LyricsTest, LookupTwiceContinuously) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  VerifyLyricAtOffset(lyrics.get(), 60, 70, "Line:60");
  VerifyLyricAtOffset(lyrics.get(), 80, 100, "Line:70");
}

TEST_F(LyricsTest, LookupMiddleThenLeft) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  VerifyLyricAtOffset(lyrics.get(), 60, 70, "Line:60");
  VerifyLyricAtOffset(lyrics.get(), 30, 40, "Line:20");
}

TEST_F(LyricsTest, LookupMiddleThenRight) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  VerifyLyricAtOffset(lyrics.get(), 40, 60, "Line:40");
  VerifyLyricAtOffset(lyrics.get(), 80, 100, "Line:70");
}

TEST_F(LyricsTest, LinearLookup) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  std::array<std::string, 12> lyric_texts {
    "",  // 0
    "",  // 10
    "Line:20",
    "Line:20",
    "Line:40",
    "Line:40",
    "Line:60",
    "Line:70",
    "Line:70",
    "Line:70",
    "Line:100",  // 100
    "Line:100"  // 110
  };

  for (int i = 0; i < lyric_texts.size(); i++) {
    int offset = i * 10;
    EXPECT_EQ(lyric_texts[i], lyrics->LyricAt(offset).lyric);
  }
}

TEST_F(LyricsTest, RandomLookup) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  std::array<std::tuple<uint32_t, std::string>, 11> lookups {
    std::make_tuple(25, "Line:20"),
    std::make_tuple(200, "Line:100"),
    std::make_tuple(60, "Line:60"),
    std::make_tuple(81, "Line:70"),
    std::make_tuple(65, "Line:60"),
    std::make_tuple(100, "Line:100"),
    std::make_tuple(40, "Line:40"),
    std::make_tuple(0, ""),
    std::make_tuple(7, ""),
    std::make_tuple(50, "Line:40"),
    std::make_tuple(92, "Line:70")
  };

  for (const auto& lookup : lookups) {
    EXPECT_EQ(std::get<1>(lookup), lyrics->LyricAt(std::get<0>(lookup)).lyric);
  }
}

TEST_F(LyricsTest, PositiveAdjustment) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(10);

  EXPECT_EQ("", lyrics->LyricAt(0).lyric);
  EXPECT_EQ("", lyrics->LyricAt(20).lyric);
  EXPECT_EQ("Line:20", lyrics->LyricAt(30).lyric);
  EXPECT_EQ("Line:100", lyrics->LyricAt(999).lyric);
  EXPECT_EQ("Line:60", lyrics->LyricAt(70).lyric);
  EXPECT_EQ("Line:70", lyrics->LyricAt(80).lyric);
}

TEST_F(LyricsTest, NegativeAdjustment) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(-10);

  EXPECT_EQ("", lyrics->LyricAt(0).lyric);
  EXPECT_EQ("Line:20", lyrics->LyricAt(10).lyric);
  EXPECT_EQ("Line:100", lyrics->LyricAt(999).lyric);
  EXPECT_EQ("Line:60", lyrics->LyricAt(50).lyric);
  EXPECT_EQ("Line:70", lyrics->LyricAt(60).lyric);
}

TEST_F(LyricsTest, GreaterNegativeAdjustment) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(-30);

  EXPECT_EQ("Line:20", lyrics->LyricAt(0).lyric);
  EXPECT_EQ("Line:40", lyrics->LyricAt(10).lyric);
  EXPECT_EQ("Line:100", lyrics->LyricAt(999).lyric);
  EXPECT_EQ("Line:60", lyrics->LyricAt(30).lyric);
  EXPECT_EQ("Line:70", lyrics->LyricAt(40).lyric);
}

TEST_F(LyricsTest, InvalidOffset) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(-10);

  EXPECT_EQ("", lyrics->LyricAt(-1).lyric);
  EXPECT_EQ(lyrics->IteratorEnd(), lyrics->LyricIteratorAt(-1));
  EXPECT_EQ("", lyrics->LyricAt(Lyrics::kEndTimeNever).lyric);
  EXPECT_EQ(lyrics->IteratorEnd(),
            lyrics->LyricIteratorAt(Lyrics::kEndTimeNever));
}

}
