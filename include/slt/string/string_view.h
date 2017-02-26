#ifndef SLT_STRING_STRING_VIEW_H
#define SLT_STRING_STRING_VIEW_H

#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include "slt/debug/assert.h"
#include "slt/mem/data_block.h"
#include "slt/mem/data_view.h"

namespace slt {

class StringView {
 public:
  using value_type = char;
  using size_type = std::size_t;

  StringView() : begin_(nullptr), length_(0) {}
  StringView(char const& ch) : begin_(&ch), length_(1) {}
  StringView(StringView const& str)
      : begin_(str.begin_), length_(str.length_) {}
  StringView(char const* str) : begin_(str), length_(std::strlen(str)) {}
  StringView(char const* from, char const* to)
      : begin_(from), length_(to - from) {}
  StringView(char const* str, size_type len) : begin_(str), length_(len) {}
  StringView(std::basic_string<char> const& str)
      : begin_(str.data()), length_(str.length()) {}
  StringView(DataBlock const& data)
      : begin_(data.data()), length_(data.size()) {}
  StringView(DataView const& data)
      : begin_(data.data()), length_(data.size()) {}

  // Convertes the StringView into a string.
  std::string toString() const {
    return std::string(begin_, begin_ + length_);
  }

  char const* data() const {
    return begin_;
  }

  size_type length() const {
    return length_;
  }

  bool empty() const {
    return length_ == 0;
  }

  DataView asDataView() const {
    return DataView(begin_, length_);
  }
  explicit operator std::string() const {
    return toString();
  }

  char operator[](size_type index) const {
    SLT_ASSERT_GT(length_, index);
    return begin_[index];
  }

  StringView& operator++() {
    SLT_ASSERT_GT(length_, 0);
    --length_;
    ++begin_;

    return *this;
  }

  StringView substr(size_type pos) {
    SLT_ASSERT_GT(length_, pos);
    StringView result = *this;
    result.length_ -= pos;
    result.begin_ += pos;
    return result;
  }

  char const* begin_;
  size_type length_;
};

inline char const* begin(StringView const& str) {
  return str.begin_;
}

inline char const* end(StringView const& str) {
  return str.begin_ + str.length_;
}

inline auto rbegin(StringView const& str) {
  return std::reverse_iterator<const char*>(str.begin_ + str.length_);
}

inline auto rend(StringView const& str) {
  return std::reverse_iterator<const char*>(str.begin_);
}

inline std::size_t length(StringView str) {
  return str.length_;
}

inline std::string& operator+=(std::string& lhs, StringView const& rhs) {
  lhs += rhs.toString();
  return lhs;
}

inline bool operator==(StringView const& lhs, StringView const& rhs) {
  return lhs.toString() == rhs.toString();
}

inline bool operator<(std::string const& lhs, StringView const& rhs) {
  return lhs < rhs.toString();
}

inline bool operator<(StringView const& lhs, std::string const& rhs) {
  return lhs.toString() < rhs;
}

inline StringView lstrip(StringView str) {
  while(str.length_ > 0) {
    auto chr = str[0];
    if(chr == ' ' || chr == '\n' || chr == '\r' || chr == '\t') {
      ++str;
    } else {
      break;
    }
  }
  return str;
}

inline bool startsWith(StringView str, StringView prefix) {
  if(length(prefix) > length(str)) {
    return false;
  }

  auto src = begin(str);
  auto cmp = begin(prefix);
  auto finish = end(prefix);

  for(; cmp != finish; ++src, ++cmp) {
    if(*cmp != *src) {
      return false;
    }
  }

  return true;
}

inline bool endsWith(StringView str, StringView prefix) {
  if(length(prefix) > length(str)) {
    return false;
  }

  auto src = rbegin(str);
  auto cmp = rbegin(prefix);
  auto finish = rend(prefix);

  for(; cmp != finish; ++src, ++cmp) {
    if(*cmp != *src) {
      return false;
    }
  }

  return true;
}

inline std::pair<StringView, StringView> find(StringView str, char sep) {
  unsigned int offset = 0;
  for(auto c : str) {
    if(c == sep) {
      break;
    }
    ++offset;
  }

  if(offset == str.length()) {
    return std::pair<StringView, StringView>();
  }

  return std::make_pair(
      StringView(str.begin_, offset),
      StringView(str.begin_ + offset + 1, str.length() - offset - 1));
}
}

inline std::ostream& operator<<(std::ostream& stream, slt::StringView string) {
  stream << string.toString();
  return stream;
}

#endif
