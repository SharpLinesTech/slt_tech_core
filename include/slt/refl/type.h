#ifndef SLT_REFL_TYPE_H
#define SLT_REFL_TYPE_H

#include "slt/mem/alignment.h"
#include "slt/mem/data_view.h"
#include "slt/refl/traits.h"

namespace slt {
namespace refl {
class TypeLib;

class Type {
 public:
  Type();
  virtual ~Type();

  virtual void destruct(MutableDataView dst) const = 0;
  virtual slt::mem::SizeAndAlignment dataSize(
      DataView obj, SerializationFormat fmt) const = 0;

  virtual MutableDataView toData(DataView obj, MutableDataView dst,
                                 SerializationFormat fmt) const = 0;

  virtual DataView assignFromData(MutableDataView obj, DataView data,
                                  SerializationFormat fmt) const = 0;
};

// This should be specialized for each category of type.
template <typename T>
class TypeImpl : public Type {};

// Implementation of common utilities shared by all types.
template <typename T>
class TypeImplCommon : public Type {
 public:
  TypeImplCommon(TypeLib*) {}

  void destruct(MutableDataView obj) const override {
    SLT_ASSERT_EQ(obj.size(), sizeof(T));
    reinterpret_cast<T const*>(obj.data())->~T();
  }

  slt::mem::SizeAndAlignment dataSize(DataView obj,
                                      SerializationFormat fmt) const override {
    SLT_ASSERT_EQ(obj.size(), sizeof(T));
    return refl::Traits<T>::dataSize(*reinterpret_cast<T const*>(obj.data()),
                                     fmt);
  }

  MutableDataView toData(DataView obj, MutableDataView dst,
                         SerializationFormat fmt) const override {
    SLT_ASSERT_EQ(obj.size(), sizeof(T));
    return refl::Traits<T>::toData(*reinterpret_cast<T const*>(obj.data()), dst,
                                   fmt);
  }

  DataView assignFromData(MutableDataView obj, DataView data,
                          SerializationFormat fmt) const override {
    SLT_ASSERT_EQ(obj.size(), sizeof(T));
    return refl::Traits<T>::assignFromData(*reinterpret_cast<T*>(obj.data()),
                                           data, fmt);
  }
};
}
}

#endif