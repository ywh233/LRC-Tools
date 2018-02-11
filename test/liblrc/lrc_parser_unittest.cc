//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 09 2018
//  File : lrc_parser_unittest.cc
//******************************************

#include "liblrc/lrc_parser.h"

#include <array>
#include <memory>

#include "gtest/gtest.h"

namespace lrc {

namespace {

const char* const kTestLrc = R"(
[ar:Artist]
[al:Album]
[ti:Title]
[au:Lyrics Writer]
[by:File Creator]
[offset:10]

[00:00.10]L1
[00:05.00][01:20.35]L2

[00:10.00][01:40.20]L3)";

int32_t Offset(int minute, float second) {
  // +10: adjustment
  return minute * 60000 + static_cast<int>(second * 1000) + 10;
}

}  // namespace

TEST(LrcParserTest, VerifyMetadata) {
  LrcParser parser;
  std::unique_ptr<Lyrics> lyrics = parser.Parse(kTestLrc);
  EXPECT_EQ("Artist", lyrics->GetMetadata().artist);
  EXPECT_EQ("Album", lyrics->GetMetadata().album);
  EXPECT_EQ("Title", lyrics->GetMetadata().title);
  EXPECT_EQ("Lyrics Writer", lyrics->GetMetadata().lyricsWriter);
  EXPECT_EQ("File Creator", lyrics->GetMetadata().fileCreator);
  EXPECT_EQ(10, lyrics->GetMetadata().adjustment);
}

TEST(LrcParserTest, VerifyLyricLines) {
  std::array<Lyrics::LyricLine, 6> expected_lyrics = {
    Lyrics::LyricLine {0, Offset(0, 0.1), ""},
    Lyrics::LyricLine {Offset(0, 0.1), Offset(0, 5), "L1"},
    Lyrics::LyricLine {Offset(0, 5), Offset(0, 10), "L2"},
    Lyrics::LyricLine {Offset(0, 10), Offset(1, 20.35), "L3"},
    Lyrics::LyricLine {Offset(1, 20.35), Offset(1, 40.2), "L2"},
    Lyrics::LyricLine {Offset(1, 40.2), Lyrics::kEndTimeNever, "L3"}
  };

  LrcParser parser;
  std::unique_ptr<Lyrics> lyrics = parser.Parse(kTestLrc);

  Lyrics::ConstLyricIterator iter = lyrics->IteratorBegin();
  for (const Lyrics::LyricLine& expect_line : expected_lyrics) {
    const Lyrics::LyricLine& actual_line = *iter;
    EXPECT_EQ(expect_line.start_time, actual_line.start_time);
    EXPECT_EQ(expect_line.end_time, actual_line.end_time);
    EXPECT_EQ(expect_line.lyric, actual_line.lyric);

    iter++;
  }
  EXPECT_EQ(lyrics->IteratorEnd(), iter);
}

}  // namespace lrc
