#ifndef SLT_REFL_REFLECTED_H
#define SLT_REFL_REFLECTED_H

#include "slt/mem/data_block.h"
#include "slt/mem/data_view.h"
#include "slt/refl/type.h"

namespace slt {
namespace refl {
struct Reflected {
  Type const* type;
  DataView object;
};

struct MutableReflected {
  Type const* type;
  MutableDataView object;
};

class ReflectedBuffer {
 public:
  template <typename T>
  ReflectedBuffer(Type const* type, T&& val)
      : type_(type), data_(sizeof(std::remove_reference_t<T>)) {
    new(data_.data()) std::remove_reference_t<T>(std::forward<T>(val));
  }

  ReflectedBuffer(ReflectedBuffer&& rhs)
      : type_(rhs.type_), data_(std::move(rhs.data_)) {}

  ReflectedBuffer& operator=(ReflectedBuffer&& rhs) {
    type_ = rhs.type_;
    data_ = std::move(rhs.data_);
    return *this;
  }

  ~ReflectedBuffer() {
    if(data_) {
      type_->destruct(data_.data());
    }
  }

  Type const* getType() const {
    return type_;
  }

  ReflectedBuffer(ReflectedBuffer const&) = delete;
  ReflectedBuffer& operator=(ReflectedBuffer const&) = delete;

  char* data() {
    return data_.data();
  }

 private:
  Type const* type_;
  DataBlock data_;
};
}
}

#endif