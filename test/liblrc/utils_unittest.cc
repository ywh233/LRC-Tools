//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 09 2018
//  File : utils_unittest.cc
//******************************************

#include "liblrc/utils.h"

#include "gtest/gtest.h"

namespace lrc {

TEST(LrcUtilsTest, VerifyTimeStringToMilliseconds) {
  // Invalid values.
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds(""));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("0:0"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("1:0"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("123abc"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("12:abc:3"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("12:70"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("123:4"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("123:4.5"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("123:45:6"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("123.45"));
  EXPECT_EQ(-1, utils::TimeStringToMilliseconds("123:45.6.7"));

  // Valid values.
  EXPECT_EQ(0, utils::TimeStringToMilliseconds("0:00"));
  EXPECT_EQ(0, utils::TimeStringToMilliseconds("0:00.0"));
  EXPECT_EQ(0, utils::TimeStringToMilliseconds("0:00.00"));
  EXPECT_EQ(1000, utils::TimeStringToMilliseconds("0:01"));
  EXPECT_EQ(1000, utils::TimeStringToMilliseconds("0:01.0"));
  EXPECT_EQ(1110, utils::TimeStringToMilliseconds("0:01.11"));
  EXPECT_EQ(60000, utils::TimeStringToMilliseconds("1:00"));
  EXPECT_EQ(61000, utils::TimeStringToMilliseconds("1:01"));
  EXPECT_EQ(61100, utils::TimeStringToMilliseconds("1:01.1"));
  EXPECT_EQ(61100, utils::TimeStringToMilliseconds("1:01.10"));
  EXPECT_EQ(61110, utils::TimeStringToMilliseconds("1:01.11"));
  EXPECT_EQ(61111, utils::TimeStringToMilliseconds("1:01.111"));
  EXPECT_EQ(61111, utils::TimeStringToMilliseconds("1:01.1111"));
}

}  // namespace lrc
