//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 09 2018
//  File : main.cc
//******************************************

#include "gtest/gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}
