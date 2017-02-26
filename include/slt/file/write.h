#ifndef SLT_FILE_WRITE_H
#define SLT_FILE_WRITE_H

#include "slt/mem/data_view.h"
#include "slt/string/string_view.h"

namespace slt {
namespace file {
struct WriteError : public std::runtime_error {
  WriteError(std::string);
};

// Synchronously writes a chunk of data to a file.
void write(DataView data, StringView file);
}
}

#endif
