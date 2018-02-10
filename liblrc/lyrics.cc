//******************************************
//  Author : Yuwei Huang
//  Created On : Thu Feb 08 2018
//  File : lyrics.cc
//******************************************

#include "liblrc/lyrics.h"

#include <assert.h>

#include <algorithm>

namespace lrc {

namespace {

bool CompareLyricStartTime(const Lyrics::LyricLine& l1,
                           const Lyrics::LyricLine& l2) {
  return l1.start_time < l2.start_time;
}

bool IsLyricCurrent(const Lyrics::LyricLine& lyric, uint32_t now) {
  return lyric.start_time <= now && now < lyric.end_time;
}

std::vector<Lyrics::LyricLine>::iterator BinaryFindLyric(
    const std::vector<Lyrics::LyricLine>::iterator& first,
    const std::vector<Lyrics::LyricLine>::iterator& last_excl,
    const std::vector<Lyrics::LyricLine>::iterator& value_if_not_found,
    uint32_t offset) {
  // For sorting purpose.
  Lyrics::LyricLine helperLine {offset, 0u, ""};
  auto iter = std::upper_bound(first, last_excl, helperLine,
                               &CompareLyricStartTime) - 1;
  if (!IsLyricCurrent(*iter, offset)) {
    iter = value_if_not_found;
  }
  return iter;
}

}  // namespace

// static
const uint32_t Lyrics::kEndTimeNever =
    std::numeric_limits<uint32_t>::max() >> 1;

Lyrics::Lyrics(const Metadata& metadata, std::vector<LyricLine>&& lyrics)
    : metadata_(metadata),
      lyrics_(std::move(lyrics)) {
  std::sort(lyrics_.begin(), lyrics_.end(), &CompareLyricStartTime);

  // Filling durations.
  for (auto it = lyrics_.begin(); it != lyrics_.end(); it++) {
    auto it_next = it+1;
    if (it_next == lyrics_.end()) {
      it->end_time = kEndTimeNever;
    } else {
      it->end_time = it_next->start_time;
      assert(it->end_time < kEndTimeNever);
    }
  }

  current_lyric_ = lyrics_.begin();
}

Lyrics::~Lyrics() {}

const Lyrics::Metadata& Lyrics::GetMetadata() const {
  return metadata_;
}

const Lyrics::LyricLine& Lyrics::LyricAt(uint32_t offset) {
  return *LyricIteratorAt(offset);
}

Lyrics::ConstLyricIterator Lyrics::LyricIteratorAt(uint32_t offset) {
  // + delays lyrics, which means making the offset sooner.
  offset -= metadata_.adjustment;

  // Empty lyric. Nothing we can do.
  if (current_lyric_ == lyrics_.end()) {
    return current_lyric_;
  }

  // Lookup lyrics after current lyric.
  if (offset >= current_lyric_->end_time) {
    if (current_lyric_ + 1 == lyrics_.end()) {
      // Already at the end. No need to move forward.
      return current_lyric_;
    }
    // Check if the next lyric is current before doing binary search. Optimized
    // for linear lookup.
    current_lyric_++;
    if (IsLyricCurrent(*current_lyric_, offset)) {
      return current_lyric_;
    }
    // Binary search on [Previous + 2, end).
    current_lyric_++;
    current_lyric_ =
        BinaryFindLyric(current_lyric_, lyrics_.end(), lyrics_.end(), offset);
    return current_lyric_;
  }

  // Lookup lyrics before current lyric.
  if (offset < current_lyric_->start_time) {
    // Binary search on [begin, current).
    current_lyric_ =
        BinaryFindLyric(lyrics_.begin(), current_lyric_, lyrics_.end(), offset);
    return current_lyric_;
  }

  // Lyric is still current.
  assert(IsLyricCurrent(*current_lyric_, offset));

  return current_lyric_;
}

Lyrics::ConstLyricIterator Lyrics::IteratorBegin() const {
  return lyrics_.cbegin();
}

Lyrics::ConstLyricIterator Lyrics::IteratorEnd() const {
  return lyrics_.cend();
}

}  // namespace lrc
