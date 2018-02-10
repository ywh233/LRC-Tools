//******************************************
//  Author : Yuwei Huang
//  Created On : Thu Feb 08 2018
//  File : lyrics.h
//******************************************

#ifndef MODEL_LYRICS_H
#define MODEL_LYRICS_H

#include <cstdint>
#include <string>
#include <vector>

namespace lrc {

class Lyrics {
 public:
  struct Metadata {
    std::string artist;
    std::string album;
    std::string title;
    std::string lyricsWriter;
    std::string fileCreator;
    uint32_t length;  // in milliseconds.
    int32_t adjustment;  // in milliseconds, + shifts time up, - shifts down.
  };

  static const uint32_t kEndTimeNever;

  struct LyricLine {
    uint32_t start_time;  // in milliseconds.

    // kEndTimeNever if it is at the end of the song.
    uint32_t end_time;

    std::string lyric;
  };

  using ConstLyricIterator = std::vector<LyricLine>::const_iterator;

  // Lyric duration is not required and will be automatically filled.
  Lyrics(const Metadata& metadata, std::vector<LyricLine>&& lyrics);
  ~Lyrics();

  const Metadata& GetMetadata() const;

  const LyricLine& LyricAt(uint32_t offset);
  ConstLyricIterator LyricIteratorAt(uint32_t offset);
  ConstLyricIterator IteratorBegin() const;
  ConstLyricIterator IteratorEnd() const;

 private:
  Metadata metadata_;
  std::vector<LyricLine> lyrics_;
  std::vector<LyricLine>::iterator current_lyric_;
};

}  // namespace lrc

#endif  // MODEL_LYRICS_H
