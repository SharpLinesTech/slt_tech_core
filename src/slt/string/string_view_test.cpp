#include "gtest/gtest.h"

#include "slt/string/string_view.h"

using namespace slt;

TEST(StringView, Iterate) {
  StringView str("allo");

  std::vector<char> as_chars(begin(str), end(str));

  EXPECT_EQ('a', str[0]);
  EXPECT_EQ('a', as_chars[0]);

  EXPECT_EQ('l', str[1]);
  EXPECT_EQ('l', as_chars[1]);

  EXPECT_EQ('l', str[2]);
  EXPECT_EQ('l', as_chars[2]);

  EXPECT_EQ('o', str[3]);
  EXPECT_EQ('o', as_chars[3]);
}

TEST(StringView, ReverseIterate) {
  StringView str("allo");

  std::vector<char> as_chars(rbegin(str), rend(str));

  EXPECT_EQ('a', as_chars[3]);
  EXPECT_EQ('l', as_chars[2]);
  EXPECT_EQ('l', as_chars[1]);
  EXPECT_EQ('o', as_chars[0]);
}

TEST(StringView, StartsWith) {
  EXPECT_TRUE(startsWith("allo", "a"));
  EXPECT_TRUE(startsWith("allo", "al"));
  EXPECT_TRUE(startsWith("allo", "allo"));

  EXPECT_FALSE(startsWith("allo", "b"));
  EXPECT_FALSE(startsWith("allo", "a2"));
  EXPECT_FALSE(startsWith("allo", "allo3"));
}

TEST(StringView, EndsWith) {
  EXPECT_TRUE(endsWith("allo", "o"));
  EXPECT_TRUE(endsWith("allo", "lo"));
  EXPECT_TRUE(endsWith("allo", "allo"));

  EXPECT_FALSE(endsWith("allo", "a"));
  EXPECT_FALSE(endsWith("allo", "al"));
}

TEST(StringView, UseToFindInMap) {
  std::map<std::string, int, std::less<void>> values;

  values["Yo"] = 12;
  StringView sv = "Yo";
  EXPECT_EQ(values.find(sv)->second, 12);
}

TEST(StringView, LStrip) {
  StringView str("  allo");
  str = lstrip(str);
  EXPECT_EQ("allo", str);
  str = lstrip(str);
  EXPECT_EQ("allo", str);
}

TEST(StringView, Find) {
  StringView str("aaa:bbb");
  auto found = find(str, ':');
  EXPECT_EQ("aaa", found.first);
  EXPECT_EQ("bbb", found.second);
}

TEST(StringView, FindAtEnd) {
  StringView str("aaa:");
  auto found = find(str, ':');
  EXPECT_EQ("aaa", found.first);
  EXPECT_EQ("", found.second);
}