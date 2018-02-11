//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 09 2018
//  File : lrc_parser_unittest.cc
//******************************************

#include "liblrc/lrc_parser.h"

#include <array>
#include <memory>

#include "liblrc/utils.h"
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

int32_t Offset(const std::string& time_str) {
  // +10: adjustment
  return utils::TimeStringToMilliseconds(time_str) + 10;
}

}  // namespace

TEST(LrcParserTest, VerifyMetadata) {
  LrcParser parser;
  std::unique_ptr<Lyrics> lyrics = parser.ParseString(kTestLrc);
  EXPECT_EQ("Artist", lyrics->GetMetadata().artist);
  EXPECT_EQ("Album", lyrics->GetMetadata().album);
  EXPECT_EQ("Title", lyrics->GetMetadata().title);
  EXPECT_EQ("Lyrics Writer", lyrics->GetMetadata().lyricsWriter);
  EXPECT_EQ("File Creator", lyrics->GetMetadata().fileCreator);
  EXPECT_EQ(10, lyrics->GetMetadata().adjustment);
}

TEST(LrcParserTest, VerifyCRLF) {
  LrcParser parser;
  std::string test_lrc_crlf = std::string(kTestLrc);

  size_t start_pos = 0;
  while((start_pos =
      test_lrc_crlf.find('\n', start_pos)) != std::string::npos) {
      test_lrc_crlf.replace(start_pos, 1, "\r\n");
      start_pos += 2;  // length of \r\n
  }
  ASSERT_NE(strlen(kTestLrc), test_lrc_crlf.length());
  std::unique_ptr<Lyrics> lyrics = parser.ParseString(test_lrc_crlf);

  EXPECT_EQ("Artist", lyrics->GetMetadata().artist);
  EXPECT_EQ("Album", lyrics->GetMetadata().album);
  EXPECT_EQ("Title", lyrics->GetMetadata().title);
  EXPECT_EQ("Lyrics Writer", lyrics->GetMetadata().lyricsWriter);
  EXPECT_EQ("File Creator", lyrics->GetMetadata().fileCreator);
  EXPECT_EQ(10, lyrics->GetMetadata().adjustment);
}

TEST(LrcParserTest, VerifyLyricLines) {
  std::array<Lyrics::LyricLine, 6> expected_lyrics = {
    Lyrics::LyricLine {0, Offset("0:00.1"), ""},
    Lyrics::LyricLine {Offset("0:00.1"), Offset("0:05"), "L1"},
    Lyrics::LyricLine {Offset("0:05"), Offset("0:10"), "L2"},
    Lyrics::LyricLine {Offset("0:10"), Offset("1:20.35"), "L3"},
    Lyrics::LyricLine {Offset("1:20.35"), Offset("1:40.2"), "L2"},
    Lyrics::LyricLine {Offset("1:40.2"), Lyrics::kEndTimeNever, "L3"}
  };

  LrcParser parser;
  std::unique_ptr<Lyrics> lyrics = parser.ParseString(kTestLrc);

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
