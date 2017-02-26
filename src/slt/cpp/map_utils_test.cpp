#include "gtest/gtest.h"

#include "slt/cpp/map_utils.h"
#include "slt/string/string_view.h"

#include <map>
#include <unordered_map>

TEST(FindOrInsert, Insertion) {
  std::map<int, int> tgt;

  bool invoked = false;
  auto cb = [&]() {
    invoked = true;
    return -1;
  };

  auto res = slt::findOrInsert(tgt, 0, cb);

  EXPECT_TRUE(invoked);
  EXPECT_EQ(-1, res);
  EXPECT_EQ(-1, tgt[0]);
}

TEST(FindOrInsert, Insertion_unordered_map) {
  std::unordered_map<int, int> tgt;

  bool invoked = false;
  auto cb = [&]() {
    invoked = true;
    return -1;
  };

  auto res = slt::findOrInsert(tgt, 0, cb);

  EXPECT_TRUE(invoked);
  EXPECT_EQ(-1, res);
  EXPECT_EQ(-1, tgt[0]);
}

TEST(FindOrInsert, Collision) {
  std::map<int, int> tgt;

  tgt[0] = 12;
  bool invoked = false;
  auto cb = [&]() {
    invoked = true;
    return -1;
  };

  auto res = slt::findOrInsert(tgt, 0, cb);

  EXPECT_FALSE(invoked);
  EXPECT_EQ(12, res);
  EXPECT_EQ(12, tgt[0]);
}

TEST(FindOrInsert, StringViewToString) {
  std::map<std::string, int, std::less<>> tgt;

  tgt["a"] = 12;
  bool invoked = false;
  auto cb = [&]() {
    invoked = true;
    return -1;
  };

  auto res = slt::findOrInsert(tgt, "a", cb);
  EXPECT_FALSE(invoked);

  res = slt::findOrInsert(tgt, slt::StringView("a"), cb);
  EXPECT_FALSE(invoked);

  res = slt::findOrInsert(tgt, slt::StringView("b"), cb);
  EXPECT_TRUE(invoked);
}