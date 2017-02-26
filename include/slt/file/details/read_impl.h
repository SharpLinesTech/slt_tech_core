#ifndef SLT_FILE_READ_IMPL_H
#define SLT_FILE_READ_IMPL_H

namespace slt {
namespace file {
template <typename T>
T readObject(std::string file, refl::SerializationFormat fmt) {
  T result;
  refl::assignFromData(result, read(file), fmt);
  return result;
}

template <typename T>
void asyncReadObject(std::string const &file, refl::SerializationFormat fmt,
                     concur::EventQueue &queue, ReadObjectCallback<T> cb,
                     ReadFailedCallback err_cb) {
  auto on_read = [ cb{std::move(cb)}, fmt ](DataBlock d) {
    T object;
    refl::assignFromData(object, std::move(d), fmt);
    cb(std::move(object));
  };

  asyncRead(file, queue, std::move(on_read), std::move(err_cb));
}
}
}
#endif