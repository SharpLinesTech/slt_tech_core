#include "gtest/gtest.h"

#include "slt/debug/assert.h"

TEST(Assert, TestUnreachable) {
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_UNREACHABLE(); }());
}

TEST(Assert, TestSimpleAssert) {
  SLT_ASSERT(true);

  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT(false); }());

  // Composed expression.
  SLT_ASSERT(2 + 2 > 3);
}

TEST(Assert, TestBinaryAssertions) {
  // ==
  SLT_ASSERT_EQ(1, 1);
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_EQ(1, 2); }());
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_EQ(2, 1); }());

  // >
  SLT_ASSERT_GT(4, 1);
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_GT(4, 4); }());

  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_GT(1, 4); }());

  // >=
  SLT_ASSERT_GE(4, 1);
  SLT_ASSERT_GE(5, 5);
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_GE(1, 4); }());

  // <
  SLT_ASSERT_LT(1, 4);
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_LT(4, 4); }());
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_LT(4, 1); }());

  SLT_ASSERT_LE(1, 4);
  SLT_ASSERT_LE(4, 4);
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_LE(4, 1); }());

  SLT_ASSERT_FLOAT_EQ(0.5f, 0.5f);
  EXPECT_SLT_ASSERT_FAILURE([]() { SLT_ASSERT_FLOAT_EQ(1.0f, 2.0f); }());
}

TEST(Assert, TestStringEquality) {
  SLT_ASSERT_EQ(std::string("hello"), "hello");
  EXPECT_SLT_ASSERT_FAILURE(
      []() { SLT_ASSERT_EQ(std::string("hello"), "world"); }());
}