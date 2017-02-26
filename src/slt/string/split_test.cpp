#include "gtest/gtest.h"

#include "slt/string/split.h"

using namespace slt;

TEST(StringSplitTest, EmptyString) {
  auto splits = split("", ':');

  EXPECT_EQ(0, splits.size());
}

TEST(StringSplitTest, SimpleCase) {
  auto splits = split("ab:cd", ':');

  EXPECT_EQ(2, splits.size());
  EXPECT_EQ("ab", splits[0]);
  EXPECT_EQ("cd", splits[1]);
}

TEST(StringSplitTest, MultipleCase) {
  auto splits = split("ab:cd:ef", ':');

  EXPECT_EQ(3, splits.size());
  EXPECT_EQ("ab", splits[0]);
  EXPECT_EQ("cd", splits[1]);
  EXPECT_EQ("ef", splits[2]);
}

TEST(StringSplitTest, TrailingSeparator) {
  auto splits = split("ab:cd:", ':');

  EXPECT_EQ(3, splits.size());
  EXPECT_EQ("ab", splits[0]);
  EXPECT_EQ("cd", splits[1]);
  EXPECT_EQ("", splits[2]);
}

TEST(StringSplitTest, FrontSeparator) {
  auto splits = split(":ab:cd", ':');

  EXPECT_EQ(3, splits.size());
  EXPECT_EQ("", splits[0]);
  EXPECT_EQ("ab", splits[1]);
  EXPECT_EQ("cd", splits[2]);
}