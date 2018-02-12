# LRC-Tools

[![Build Status](https://travis-ci.org/equinox1993/LRC-Tools.svg?branch=master)](https://travis-ci.org/equinox1993/LRC-Tools)

Tools for parsing and playing back LRC lyrics.

## Usage

```C++
// Parsing file.
lrc::LrcParser parser;

// You may also use ParseString to parse a string or ParseStream to parse from an istream.
std::unique_ptr<lrc::Lyrics> lyrics = parser.ParseFile(filename);

// Getting metadata.
const lrc::Lyrics::Metadata& metadata = lyrics->GetMetadata();
std::cout << "Title: " << metadata.title << std::endl
          << "Album: " << metadata.album << std::endl
          << "Artist: " << metadata.artist << std::endl
          << "Adjustment: " << metadata.adjustment << std::endl;

// Printing lyric and other information.
const lrc::Lyrics::LyricLine& line =
    lyrics->LyricAt(lrc::utils::TimeStringToMilliseconds("01:18.29"));
std::cout << "Start time: " << line.start_time << "\t"
          << "End time: " << line.end_time << "\t"
          << "Lyric: " << line.lyric;

// Printing lyrics around current lyric.
lrc::Lyrics::ConstLyricIterator iter = lyrics->LyricAt(78290);
//   Previous lyric
// > Current lyric
//   Next lyric
std::cout << "  " << (iter - 1)->lyric << std::endl
          << "> " << iter->lyric << std::endl
          << "  " << (iter + 1)->lyric << std::endl;
```

You may also see code in `cli/` as an example.

## Requirements

* Compiler that supports C++11 standard.
* CMake > 3.2.

## Project Structure

* **liblrc**: Library for parsing LRC files and looking up lyrics at a given time.
* **cli**: Command line tools for querying LRC metadata or lyrics at a given time. Depends on liblrc.

## Building

In project root directory:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

Then libraries and programs will be built in their corresponding directory inside `build/`.

## Testing

Follow all the steps in [Building](#building), then run:

    $ make test

## Third-party dependencies

* [GoogleTest](https://github.com/google/googletest)
* [cxxopts](https://github.com/jarro2783/cxxopts)

They will be automatically downloaded when you build the project. Note that none of them are dependency of the lrc library.
