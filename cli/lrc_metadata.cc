//******************************************
//  Author : Author
//  Created On : Sat Feb 10 2018
//  File : lrc_metadata.cc
//******************************************

#include <iostream>

#include "liblrc/lrc_parser.h"
#include "third_party/cxxopts/include/cxxopts.hpp"

void ShowHelp(const cxxopts::Options& options, int exit_code) {
  std::cout << options.help({""}) << std::endl;
  exit(exit_code);
}

int main(int argc, char** argv) {
  try {
    // Setup.
    cxxopts::Options options(argv[0], "CLI tool to print LRC metadata.");
    options.add_options()
        ("f,file", "Name of the lrc file", cxxopts::value<std::string>(),
            "FILE")
        ("h,help", "Print help");

    auto result = options.parse(argc, argv);

    // Help
    if (result.count("h")) {
      ShowHelp(options, 0);
    }

    // -f
    if (!result.count("f")) {
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

    std::cout
        << "Artist: " << lyrics->GetMetadata().artist << std::endl
        << "Album: " << lyrics->GetMetadata().album << std::endl
        << "Title: " << lyrics->GetMetadata().title << std::endl
        << "Lyrics Writer: " << lyrics->GetMetadata().lyricsWriter
            << std::endl
        << "File Creator: " << lyrics->GetMetadata().fileCreator
            << std::endl
        << "Adjustment: " << lyrics->GetMetadata().adjustment << std::endl;
  } catch (const cxxopts::OptionException& e) {
    std::cerr << "Error parsing options: " << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
