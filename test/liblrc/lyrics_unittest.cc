//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 09 2018
//  File : lyricstest.cc
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
      {0, 0, "Line:0"},
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
    999,
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
  EXPECT_EQ(999, lyrics->GetMetadata().length);
  EXPECT_EQ(-1, lyrics->GetMetadata().adjustment);
}

TEST_F(LyricsTest, LookupEmptyLyrics) {
  Lyrics::Metadata meta;
  meta.adjustment = 0;
  Lyrics lyrics(meta, {});

  EXPECT_EQ(lyrics.IteratorBegin(), lyrics.LyricIteratorAt(1));
  EXPECT_EQ(lyrics.IteratorEnd(), lyrics.LyricIteratorAt(1));
}

TEST_F(LyricsTest, LookupFirstLyric) {
  std::unique_ptr<Lyrics> lyrics = CreateTestLyrics(0);

  EXPECT_EQ(lyrics->IteratorBegin(), lyrics->LyricIteratorAt(0));
  VerifyLyricAtOffset(lyrics.get(), 0, 20, "Line:0");
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
    "Line:0",  // 0
    "Line:0",  // 10
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
    std::make_tuple(25u, "Line:20"),
    std::make_tuple(200u, "Line:100"),
    std::make_tuple(60u, "Line:60"),
    std::make_tuple(81u, "Line:70"),
    std::make_tuple(65u, "Line:60"),
    std::make_tuple(100u, "Line:100"),
    std::make_tuple(40u, "Line:40"),
    std::make_tuple(0u, "Line:0"),
    std::make_tuple(7u, "Line:0"),
    std::make_tuple(50u, "Line:40"),
    std::make_tuple(92u, "Line:70")
  };

  for (const auto& lookup : lookups) {
    EXPECT_EQ(std::get<1>(lookup), lyrics->LyricAt(std::get<0>(lookup)).lyric);
  }
}

}
