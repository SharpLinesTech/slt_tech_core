
#include "slt/string/split.h"

namespace slt {
std::vector<StringView> split(StringView str, char delim) {
  std::vector<StringView> result;

  char const* start = begin(str);
  char const* fin = end(str);

  char const* current = start;
  while(current != fin) {
    if(*current == delim) {
      result.emplace_back(start, current);
      start = current + 1;
      if(start == fin) {
        result.emplace_back();
      }
    }

    ++current;
  }

  if(start != fin) {
    result.emplace_back(start, fin);
  }
  return result;
}
}
