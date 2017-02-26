#include "gtest/gtest.h"

#include "slt/debug/validated.h"
#include "slt/mem/data_view.h"

TEST(DataView, CreateFromData) {
  int value = 4;
  const int c_value = 5;

  slt::DataView as_data(&value);
  EXPECT_EQ(sizeof(int), as_data.size());
  EXPECT_EQ((char*)&value, as_data.data());

  slt::DataView as_data_const(&c_value);
  EXPECT_EQ(sizeof(int), as_data_const.size());
  EXPECT_EQ((char*)&c_value, as_data_const.data());

  slt::DataView as_mutable_data(&value);
  EXPECT_EQ(sizeof(int), as_mutable_data.size());
  EXPECT_EQ((char*)&value, as_mutable_data.data());
}

TEST(DataView, CreateFromVector) {
  std::vector<int> values = {1, 2, 3, 4};
  const std::vector<int> const_values = {5, 6, 7, 8};

  slt::DataView as_data(values);
  EXPECT_EQ(sizeof(int) * values.size(), as_data.size());
  EXPECT_EQ((char*)values.data(), as_data.data());

  slt::DataView as_data_const(const_values);
  EXPECT_EQ(sizeof(int) * const_values.size(), as_data_const.size());
  EXPECT_EQ((char*)const_values.data(), as_data_const.data());

  slt::DataView as_mutable_data(values);
  EXPECT_EQ(sizeof(int) * values.size(), as_mutable_data.size());
  EXPECT_EQ((char*)values.data(), as_mutable_data.data());
}

TEST(DataView, CreateFromDataBlock) {
  slt::DataBlock dynamic_storage(128);
  const slt::DataBlock const_dynamic_storage(256);

  slt::DataView as_data(dynamic_storage);
  EXPECT_EQ(128, as_data.size());
  EXPECT_EQ(dynamic_storage.data(), as_data.data());

  slt::DataView as_data_const(const_dynamic_storage);
  EXPECT_EQ(256, as_data_const.size());
  EXPECT_EQ(const_dynamic_storage.data(), as_data_const.data());

  slt::DataView as_mutable_data(dynamic_storage);
  EXPECT_EQ(128, as_mutable_data.size());
  EXPECT_EQ(dynamic_storage.data(), as_mutable_data.data());
}

void validator(int const& v) {
  // only accept 5 as a value
  SLT_ASSERT_LE(v, 10);
}

using TestValidated = slt::Validated<int, validator>;

TEST(DataView, CreateFromValidated) {
  TestValidated x = 5;
  std::vector<TestValidated> v_x = {1, 2, 3, 4};

  // This should fail to compile:
   //slt::MutableDataView mut(&x);

  slt::DataView as_data(&x);
  slt::DataView as_data_v(v_x);

  EXPECT_EQ(*as_data.as<int>(), 5);
  EXPECT_EQ(*as_data_v.as<int>(), 1);
}

TEST(MutableDataView, AlterValue) {
  int target = 5;
  slt::MutableDataView as_data(&target);

  *as_data.as<int>() = 3;

  EXPECT_EQ(target, 3);
}

TEST(MutableDataView, DetectBadAlignment) {
  char raw_data[128];
  slt::MutableDataView as_data_aligned(raw_data, 128);
  slt::MutableDataView as_data_unaligned(raw_data + 1, 128);

  EXPECT_TRUE(as_data_aligned.isValid<int>());
  EXPECT_FALSE(as_data_unaligned.isValid<int>());
}

TEST(MutableDataView, DetectInsuficientStorage) {
  char raw_data[128];
  slt::MutableDataView as_data_enough(raw_data, sizeof(int));
  slt::MutableDataView as_data_missing(raw_data, sizeof(int) - 1);

  EXPECT_TRUE(as_data_enough.isValid<int>());
  EXPECT_FALSE(as_data_missing.isValid<int>());
}

TEST(MutableDataView, TestAlign) {
  int raw_data[10];
  slt::MutableDataView as_misaligned_data(((char*)&raw_data[0]) + 1,
                                          9 * sizeof(int));

  EXPECT_FALSE(as_misaligned_data.isValid<int>());
  auto realigned = as_misaligned_data.align(alignof(int));

  EXPECT_TRUE(realigned.isValid<int>());
  EXPECT_EQ(8 * sizeof(int) + 1, realigned.size());
  EXPECT_EQ((char*)&raw_data[1], realigned.data());
}

TEST(MutableDataView, TestAlignDoesntTouchValid) {
  int raw_data[10];
  slt::MutableDataView as_aligned_data((char*)&raw_data[0], 10 * sizeof(int));

  EXPECT_TRUE(as_aligned_data.isValid<int>());
  auto realigned = as_aligned_data.align(alignof(int));

  EXPECT_EQ(as_aligned_data.size(), realigned.size());
  EXPECT_EQ(as_aligned_data.data(), realigned.data());
}

TEST(DataView, TestAlign) {
  int raw_data[10];
  slt::DataView as_misaligned_data(((char*)&raw_data[0]) + 1, 9 * sizeof(int));

  EXPECT_FALSE(as_misaligned_data.isValid<int>());
  auto realigned = as_misaligned_data.align(alignof(int));

  EXPECT_TRUE(realigned.isValid<int>());
  EXPECT_EQ(8 * sizeof(int) + 1, realigned.size());
  EXPECT_EQ((char*)&raw_data[1], realigned.data());
}

TEST(DataView, TestAlignDoesntTouchValid) {
  int raw_data[10];
  slt::DataView as_aligned_data((char*)&raw_data[0], 10 * sizeof(int));

  EXPECT_TRUE(as_aligned_data.isValid<int>());
  auto realigned = as_aligned_data.align(alignof(int));

  EXPECT_EQ(as_aligned_data.size(), realigned.size());
  EXPECT_EQ(as_aligned_data.data(), realigned.data());
}

TEST(DataView, NonPODObject) {
  std::string object = "allo";
  slt::DataView as_data_view(&object);

  EXPECT_EQ((std::string*)as_data_view.data(), &object);
}
