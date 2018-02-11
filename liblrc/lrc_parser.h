//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 10 2018
//  File : lrc_parser.h
//******************************************

#ifndef LIBLRC_LRC_PARSER_H
#define LIBLRC_LRC_PARSER_H

#include <memory>

#include "lyrics.h"

namespace lrc {

class LrcParser {
 public:
  LrcParser();
  ~LrcParser();

  std::unique_ptr<Lyrics> Parse(const std::string& lrc_string) const;

 private:
  LrcParser(const LrcParser&) = delete;
  LrcParser(LrcParser&&) = delete;
};

}  // namespace lrc

#endif  // LIBLRC_LRC_PARSER_H
