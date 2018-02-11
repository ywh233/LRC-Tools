//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 10 2018
//  File : lrc_parser.cc
//******************************************

#include "lrc_parser.h"

#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace lrc {

namespace {

// G1: metadata key, G2: metadata value
static const std::regex kMetadataExp(R"exp(\[([a-zA-Z]+):([^\]]+)\])exp");

// G1: lyric
static const std::regex kLyricExp(R"exp(\]([^\]]*)\r?$)exp");

// G1: mm, G2: ss
static const std::regex kTimestampExp(R"exp(\[(\d\d):(\d\d\.\d\d)\])exp");

void SetMetadata(Lyrics::Metadata* metadata,
                 const std::string& key,
                 const std::string& val) {
  if (key == "ar")
    metadata->artist = val;
  else if (key == "al")
    metadata->album = val;
  else if (key == "ti")
    metadata->title = val;
  else if (key == "au")
    metadata->lyricsWriter = val;
  else if (key == "by")
    metadata->fileCreator = val;
  else if (key == "offset") {
    metadata->adjustment = std::stoi(val);
  } else {
    // Unknown key. Do nothing.
  }
}

}  // namespace

LrcParser::LrcParser() {}
LrcParser::~LrcParser() {}

std::unique_ptr<Lyrics> LrcParser::Parse(
    const std::string& lrc_string) const {
  Lyrics::Metadata metadata;
  std::vector<Lyrics::LyricLine> lyric_lines;

  std::stringstream stream(lrc_string);
  for (std::string line; std::getline(stream, line); ) {
    std::smatch metadata_match;
    // Assume one metadata tag per line.
    if (std::regex_search(line, metadata_match, kMetadataExp)) {
      std::string key = metadata_match[1];
      std::string val = metadata_match[2];
      SetMetadata(&metadata, key, val);
      continue;
    }

    std::smatch lyric_match;
    if (!std::regex_search(line, lyric_match, kLyricExp)) {
      // No lyric. The line is malformed.
      continue;
    }
    std::string lyric = lyric_match[1];

    // Load timestamps and push lyric lines.
    std::string::const_iterator search_start(line.cbegin());
    std::smatch timestamp_match;
    while (regex_search(
        search_start, line.cend(), timestamp_match, kTimestampExp)) {

      int minutes = std::stoi(timestamp_match[1]);
      float seconds = std::stof(timestamp_match[2]);
      int millisecond_sum = 60000 * minutes + static_cast<int>(1000 * seconds);

      Lyrics::LyricLine lyric_line;
      lyric_line.start_time = millisecond_sum;
      lyric_line.lyric = lyric;
      lyric_lines.push_back(lyric_line);

      search_start += timestamp_match.position() + timestamp_match.length();
    }
  }
  return std::make_unique<Lyrics>(metadata, std::move(lyric_lines));
}

}  // namespace lrc
