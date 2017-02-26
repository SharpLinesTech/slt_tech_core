#ifndef SLT_FILE_READ_H
#define SLT_FILE_READ_H

#include <functional>
#include <stdexcept>
#include "slt/concur/event_queue.h"
#include "slt/mem/data_block.h"
#include "slt/refl/refl.h"
#include "slt/settings/settings.h"

namespace slt {

namespace file {

struct ReadError : public std::runtime_error {
  ReadError(std::string);
};

using ReadCallback = std::function<void(DataBlock)>;
template <typename T>
using ReadObjectCallback = std::function<void(T)>;

using ReadFailedCallback = std::function<void(ReadError)>;

// Synchronously loads a file from the native filesystem.
// Consider using async_load instead.
// This will throw ReadError on failure.
DataBlock read(std::string const &file);

// Synchronously load data from a file and interprets it into
// a new instance of T.
template <typename T>
inline T readObject(std::string file, refl::SerializationFormat fmt);

// Asynchronously loads file from the native filesystem.
// cb or err_cb will be put into queue upon completion.
void asyncRead(std::string file, concur::EventQueue &queue, ReadCallback cb,
               ReadFailedCallback err_cb = nullptr);

// Asynchronously loads file and interprets it into
// a new instance of T.
// cb or err_cb will be put into queue upon completion.
template <typename T>
inline void asyncReadObject(std::string const &file,
                            refl::SerializationFormat fmt,
                            concur::EventQueue &queue, ReadObjectCallback<T> cb,
                            ReadFailedCallback err_cb = nullptr);
}
}

#include "slt/file/details/read_impl.h"

#endif