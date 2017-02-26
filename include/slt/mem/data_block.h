#ifndef SLT_MEMORY_DATA_BLOCK_H
#define SLT_MEMORY_DATA_BLOCK_H

#include <cstring>
#include "slt/debug/assert.h"
#include "slt/mem/alignment.h"

namespace slt {

// Defines a movable raw block of memory.
class DataBlock {
 public:
  DataBlock() : length_(0), data_(nullptr) {}
  explicit DataBlock(std::size_t size) : length_(size), data_(new char[size]) {}
  explicit DataBlock(mem::SizeAndAlignment spec)
      : length_(spec.size), data_(new char[spec.size]) {
    // Double check, in case a sneaky memory system is being used.
    SLT_ASSERT(mem::isAligned(data_, spec.align));
  }

  DataBlock(DataBlock&& rhs) : length_(rhs.length_), data_(rhs.data_) {
    rhs.data_ = nullptr;

    rhs.length_ = 0;
  }

  ~DataBlock() {
    if(data_) {
      delete[] data_;
    }
  }

  DataBlock& operator=(DataBlock&& rhs) {
    length_ = rhs.length_;
    data_ = rhs.data_;
    rhs.data_ = nullptr;
    rhs.length_ = 0;
    return *this;
  }

  size_t size() const {
    return length_;
  }

  char const* data() const {
    return data_;
  }

  char* data() {
    return data_;
  }

  operator bool() const {
    return length_ > 0;
  }
  // These are only implemented to accomodate std::function's requirements, but
  // should never be called.
  DataBlock(DataBlock const& rhs) {
    SLT_UNREACHABLE();
  }
  DataBlock& operator=(DataBlock const& rhs) {
    SLT_UNREACHABLE();
  }

 private:
  std::size_t length_;
  char* data_;
};
}
#endif