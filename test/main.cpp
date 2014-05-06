/**
 *  Main.cpp
 *
 *  Start point of the unit tests
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2014 Copernica BV
 */

#include <gtest/gtest.h>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_shuffle = true;
  return RUN_ALL_TESTS();
};