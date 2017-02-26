#ifndef SLT_REFL_TRAITS_H
#define SLT_REFL_TRAITS_H

#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>
#include "slt/mem/alignment.h"
#include "slt/mem/data_view.h"

namespace slt {
namespace refl {

struct DecodeError : public std::runtime_error {
  DecodeError(std::string explanation);
};

struct EncodeError : public std::runtime_error {
  EncodeError(std::string explanation);
};

enum class SerializationFormat {
  // Serialization and Desrializatoin MUST be done using the same code.
  // This format is optimized for speed, not size.
  RAW_UNVERIFIED_DATA,

  // Human-readable, no efforts will be made to make this particularly fast.
  HUMAN_READABLE
};

// One would think we could use std::is_trivially_copyable<> instead,
// but this allows for types to opt out of copying behavior. This can make
// sense if a struct does not want to expose all of its fields for example.
template <typename T>
struct CanBeMemCopied : std::is_trivially_copyable<T> {};

// By default, slt::refl doesn't know what to do with a type.
template <typename T, typename Enable = void>
struct Traits {};

// Traits implementation can inherit from this to reduce boilerplate.
template <typename T>
struct TraitsBase {
  static std::string typeKey() {
    return typeid(T).name();
  }
};

// Determines the necessary amount of storage, and required alignment
// for serialization.
template <typename T>
slt::mem::SizeAndAlignment dataSize(T const& src, SerializationFormat fmt) {
  return verified(Traits<T>::dataSize(src, fmt));
}

// Serialize.
template <typename T>
MutableDataView toData(T const& src, MutableDataView dst,
                       SerializationFormat fmt) {
  return Traits<T>::toData(src, dst, fmt);
}

// Deserialize.
template <typename T>
DataView assignFromData(T& dst, DataView data, SerializationFormat fmt) {
  return Traits<T>::assignFromData(dst, data, fmt);
}

template <typename T>
DataBlock serialize(T const& src, SerializationFormat fmt) {
  auto buf_size = dataSize(src, fmt);
  DataBlock result(buf_size);
  toData(src, result, fmt);
  return result;
}
}
}

#endif