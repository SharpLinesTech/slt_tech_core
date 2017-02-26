#ifndef SLT_STRING_JOIN_H
#define SLT_STRING_JOIN_H

#include <string>
#include "slt/string/string_view.h"

namespace slt {

// TODO: This should return a custom range.
template <typename RANGE_T, typename DELIM>
std::string join(RANGE_T const& range, DELIM sep) {
  std::string result;

  auto next = begin(range);
  auto finish = end(range);

  while(next != finish) {
    auto after = next;
    ++after;

    result += *next;
    if(after != finish) {
      result += sep;
    }
    next = after;
  }

  return result;
}
}
#endif
