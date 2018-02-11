//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 10 2018
//  File : utils.h
//******************************************

#ifndef LIBLRC_UTILS_H
#define LIBLRC_UTILS_H

#include <cstdint>
#include <string>
#include <vector>

namespace lrc {

namespace utils {

// Converts strings like "mm:ss" and "mm:ss.cc" to milliseconds. Returns -1
// if |time| is not valid time string.
int32_t TimeStringToMilliseconds(const std::string& time_str);

std::string MillisecondsToTimeString(int32_t milliseconds);

}  // namespace utils

}  // namespace lrc

#endif  // LIBLRC_UTILS_H
