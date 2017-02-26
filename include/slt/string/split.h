#ifndef SLT_STRING_SPLIT_H
#define SLT_STRING_SPLIT_H

#include <vector>
#include "slt/string/string_view.h"

namespace slt {
std::vector<StringView> split(StringView str, char delim);
}
#endif
