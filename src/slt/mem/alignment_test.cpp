#include "gtest/gtest.h"

#include "slt/mem/alignment.h"

TEST(IsAligned, TestWorks) {
  int data = 3;
  char* data_as_ptr = reinterpret_cast<char*>(&data);

  EXPECT_TRUE(slt::mem::isAligned(data_as_ptr, alignof(int)));

  for(int i = 1; i < alignof(int); ++i) {
    EXPECT_FALSE(slt::mem::isAligned(data_as_ptr + i, alignof(int)));
  }

  EXPECT_TRUE(slt::mem::isAligned(data_as_ptr + alignof(int), alignof(int)));
}

TEST(AlignSize, TestWorks) {
  EXPECT_EQ(0, slt::mem::alignSize(0, 16));
  for(int i = 1; i <= 16; ++i) {
    EXPECT_EQ(16, slt::mem::alignSize(i, 16));
  }
  EXPECT_EQ(32, slt::mem::alignSize(17, 16));
}