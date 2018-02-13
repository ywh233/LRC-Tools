//******************************************
//  Author : Author
//  Created On : Sat Feb 10 2018
//  File : lrc_info.cc
//******************************************

#include <iostream>

#include "liblrc/lrc_parser.h"
#include "liblrc/utils.h"
#include "third_party/cxxopts/include/cxxopts.hpp"

void ShowHelp(const cxxopts::Options& options, int exit_code) {
  std::cout << options.help({""}) << std::endl;
  exit(exit_code);
}

void PrintLyric(const lrc::Lyrics::LyricLine& lyric,
                bool is_full_info,
                const std::string& prefix) {
  std::cout << prefix;
  if (is_full_info) {
    std::cout
        << "Starts at: "
        << lrc::utils::MillisecondsToTimeString(lyric.start_time) << "\t"
        << "Ends at: "
        << lrc::utils::MillisecondsToTimeString(lyric.end_time) << "\t"
        << "Lyric: " << lyric.lyric << std::endl;
  } else {
    std::cout << lyric.lyric << std::endl;
  }
}

int main(int argc, char** argv) {
  try {
    // Setup.
    cxxopts::Options options(argv[0], "CLI tool to print LRC info.");
    options.add_options()
        ("f,file", "Name of the lrc file", cxxopts::value<std::string>(),
            "FILE")
        ("l,lyric-at",
            "Lookup lyric at the given time, in mm:ss or mm:ss.cc format",
            cxxopts::value<std::string>(), "TIME")
        ("full", "Show full information about the lyric")
        ("lines-before", "Number of lines before the current lyric",
            cxxopts::value<int>()->default_value("0"), "N")
        ("lines-after", "Number of lines after the current lyric",
            cxxopts::value<int>()->default_value("0"), "N")
        ("h,help", "Print help");

    auto result = options.parse(argc, argv);

    // Help
    if (result.count("h")) {
      ShowHelp(options, 0);
    }

    // -f
    if (!result.count("f") || !result.count("l")) {
      ShowHelp(options, 1);
    }

    // Parse file.
    std::string filename = result["f"].as<std::string>();
    lrc::LrcParser parser;
    std::unique_ptr<lrc::Lyrics> lyrics = parser.ParseFile(filename);
    if (!lyrics) {
      std::cerr << "Failed to load lrc file: " << filename << std::endl;
      exit(1);
    }

    int32_t time =
        lrc::utils::TimeStringToMilliseconds(result["l"].as<std::string>());
    if (time < 0) {
      std::cerr << "Please check your time format." << std::endl;
      exit(1);
    }
    lrc::Lyrics::ConstLyricIterator lyric_iter =
        lyrics->LyricIteratorAt(time);
    int lines_before = result["lines-before"].as<int>();
    int lines_after = result["lines-after"].as<int>();
    if (lines_before < 0 || lines_after < 0) {
      std::cerr << "lines-before and lines-after must be >= 0." << std::endl;
      exit(1);
    }
    bool is_full_info = result.count("full");
    if (lines_before == 0 && lines_after == 0) {
      PrintLyric(*lyric_iter, is_full_info, "");
    } else {
      int begin_distance = std::distance(lyrics->IteratorBegin(), lyric_iter);
      for (int i = lines_before; i > 0; i--) {
        if (i > begin_distance) {
          // Out of bound. Just print an empty line.
          std::cout << std::endl;
          continue;
        }
        PrintLyric(*(lyric_iter - i), is_full_info, "  ");
      }

      int end_distance = std::distance(lyric_iter, lyrics->IteratorEnd());
      for (int i = 0; i < lines_after + 1; i++) {
        // Also print current lyric in this loop.
        if (i >= end_distance) {
          std::cout << std::endl;
          continue;
        }
        auto current_iter = lyric_iter + i;
        PrintLyric(*current_iter, is_full_info,
                   current_iter == lyric_iter ? "> " : "  ");
      }
    }
  } catch (const cxxopts::OptionException& e) {
    std::cerr << "Error parsing options: " << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
