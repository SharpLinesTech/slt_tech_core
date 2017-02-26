#include "gtest/gtest.h"

#include "slt/debug/assert.h"
#include "slt/refl/refl.h"

TEST(Serialize, root_value_to_binary) {
  int value = 5;
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  int new_value = 2;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, root_value_to_string) {
  int value = 5;
  auto format = slt::refl::SerializationFormat::HUMAN_READABLE;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  EXPECT_EQ(slt::StringView(data), "5");

  int new_value = 2;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, string_to_binary) {
  std::string value = "hello";
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  std::string new_value = "world";
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, string_to_string) {
  std::string value = "hello";
  auto format = slt::refl::SerializationFormat::HUMAN_READABLE;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  EXPECT_EQ(slt::StringView(data), "\"hello\"");

  std::string new_value = "world";
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, vector_to_binary) {
  std::vector<uint8_t> value = {1, 4, 2, 3};
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  std::vector<uint8_t> new_value;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, vector_to_string) {
  std::vector<int> value = {1, 4, 2, 3};
  auto format = slt::refl::SerializationFormat::HUMAN_READABLE;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  EXPECT_EQ(slt::StringView(data), "[1 4 2 3]");

  std::vector<int> new_value;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, list_to_binary) {
  std::list<int> value = {1, 4, 2, 3};
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  std::list<int> new_value;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, vector_of_vector_to_binary) {
  std::vector<std::vector<int>> value = {{1, 4, 2, 3}, {}, {1}};
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  std::vector<std::vector<int>> new_value;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(Serialize, vector_of_vector_to_string) {
  std::vector<std::vector<int>> value = {{1, 4, 2, 3}, {}, {1}};
  auto format = slt::refl::SerializationFormat::HUMAN_READABLE;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  EXPECT_EQ(slt::StringView(data), "[[1 4 2 3] [] [1]]");

  std::vector<std::vector<int>> new_value;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

enum class TestEnum {
  VAL_A,
  VAL_DANGEROUS,
  VAL_B,
  VAL_C,
};

namespace slt {
namespace refl {
REFL_ENUM_TRAIT_BEGIN(TestEnum)
REFL_ENUM_MEMBER(0, "VAL_A", TestEnum::VAL_A)
REFL_ENUM_MEMBER(1, "VERY_LONG_VAL_NAME", TestEnum::VAL_DANGEROUS)
REFL_ENUM_MEMBER(2, "VAL_B", TestEnum::VAL_B)
REFL_ENUM_MEMBER(3, "VAL_C", TestEnum::VAL_C)
REFL_ENUM_TRAIT_END()
}
}

TEST(SerializeEnum, enum_to_binary) {
  TestEnum value = TestEnum::VAL_A;
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  TestEnum new_value = TestEnum::VAL_B;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(SerializeEnum, enum_to_text) {
  TestEnum value = TestEnum::VAL_A;
  auto format = slt::refl::SerializationFormat::HUMAN_READABLE;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  EXPECT_EQ(slt::StringView(data), "VAL_A");

  TestEnum new_value = TestEnum::VAL_B;
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

TEST(SerializeEnum, enum_vector_to_binary) {
  std::vector<TestEnum> value = {TestEnum::VAL_A, TestEnum::VAL_B,
                                 TestEnum::VAL_A};
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  std::vector<TestEnum> new_value = {TestEnum::VAL_C};
  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value, new_value);
}

struct TestStruct {
  int val = 3;
  std::vector<int> vec = {1};
  TestEnum enm = TestEnum::VAL_A;
  std::string str = "hello";
};

namespace slt {
namespace refl {
REFL_STRUCT_TRAIT_BEGIN(TestStruct)
REFL_STRUCT_MEMBER(0, "val", &TestStruct::val)
REFL_STRUCT_MEMBER(1, "vec", &TestStruct::vec)
REFL_STRUCT_MEMBER(2, "enm", &TestStruct::enm)
REFL_STRUCT_MEMBER(3, "str", &TestStruct::str)
REFL_STRUCT_TRAIT_END()
}
}

TEST(SerializeEnum, struct_to_binary) {
  TestStruct value;
  auto format = slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  TestStruct new_value;
  new_value.val += 1;
  new_value.vec.push_back(2);
  new_value.enm = TestEnum::VAL_DANGEROUS;
  new_value.str = "world";

  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::RAW_UNVERIFIED_DATA);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value.val, new_value.val);
  EXPECT_EQ(value.vec, new_value.vec);
  EXPECT_EQ(value.enm, new_value.enm);
  EXPECT_EQ(value.str, new_value.str);
}

TEST(SerializeEnum, struct_to_string) {
  TestStruct value;
  auto format = slt::refl::SerializationFormat::HUMAN_READABLE;
  slt::DataBlock data(slt::refl::dataSize(value, format));
  slt::refl::toData(value, data, format);

  TestStruct new_value;
  new_value.val += 1;
  new_value.vec.push_back(2);
  new_value.enm = TestEnum::VAL_DANGEROUS;
  new_value.str = "world";

  EXPECT_EQ(slt::StringView(data),
            "{val: 3 vec: [1] enm: VAL_A str: \"hello\"}");

  auto remains = slt::refl::assignFromData(
      new_value, data, slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(0, remains.size());
  EXPECT_EQ(value.val, new_value.val);
  EXPECT_EQ(value.vec, new_value.vec);
  EXPECT_EQ(value.enm, new_value.enm);
  EXPECT_EQ(value.str, new_value.str);
}

TEST(SerializeEnum, decode_struct_pretty_string) {
  std::string data = R"(
  {
    val:12
    vec:    [1  4     5 3  ]
    enm: VAL_A
    str: "world"
  }

  )";
  TestStruct value;

  slt::refl::assignFromData(value, slt::DataView(data.data(), data.length()),
                            slt::refl::SerializationFormat::HUMAN_READABLE);

  EXPECT_EQ(value.val, 12);

  std::vector<int> expected_vec = {1, 4, 5, 3};
  EXPECT_EQ(value.vec, expected_vec);
  EXPECT_EQ(value.enm, TestEnum::VAL_A);
  EXPECT_EQ(value.str, "world");
}
