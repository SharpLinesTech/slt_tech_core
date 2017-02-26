#include "gtest/gtest.h"

#include "slt/string/join.h"

using namespace slt;

TEST(StringJoinTest, SimpleJoin) {
  std::vector<std::string> strings = {"a", "ab", "abc"};
  auto joined = join(strings, ',');

  EXPECT_EQ("a,ab,abc", joined);
}

TEST(StringJoinTest, EmptyString) {
  std::vector<std::string> strings;
  auto joined = join(strings, ',');

  EXPECT_EQ("", joined);
}
