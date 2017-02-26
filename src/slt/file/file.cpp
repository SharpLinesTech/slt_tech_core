#include <fstream>
#include "slt/concur/blocking_queue.h"
#include "slt/file/file_internal.h"
#include "slt/file/read.h"
#include "slt/file/write.h"
#include "slt/log/log.h"

namespace {
std::thread filesystem_thread;
slt::concur::BlockingQueue<std::function<bool(void)>> pending_load_operations;

void filesystemThreadMain() {
  slt::log->info("loading thread started");
  while(1) {
    auto op = pending_load_operations.pop();
    if(!op()) {
      return;
    }
  }
  slt::log->info("loading thread finished");
}
}

namespace slt {
namespace file {

ReadError::ReadError(std::string reason) : std::runtime_error(reason) {}
WriteError::WriteError(std::string reason) : std::runtime_error(reason) {}

void startFilesystemThread() {
  filesystem_thread = std::thread(filesystemThreadMain);
}

void stopFilesystemThread() {
  pending_load_operations.push([]() { return false; });
  filesystem_thread.join();
  pending_load_operations.clear();
}

DataBlock read(std::string const &file) {
  // Do Not mess with this string, it's used in log parsing.
  slt::log->info("Reading from file: {}", file);
  std::ifstream in_f(file.c_str(), std::ios::binary);
  if(!in_f.good()) {
    throw ReadError(fmt::format("Failed to load file: {}", file));
  }
  in_f.seekg(0, in_f.end);
  auto length = in_f.tellg();
  in_f.seekg(0, in_f.beg);
  DataBlock file_contents(length);
  in_f.read(file_contents.data(), length);
  return file_contents;
}

void asyncRead(std::string file, concur::EventQueue &queue, ReadCallback cb,
               ReadFailedCallback err_cb) {
  pending_load_operations.push(
      [ file, cb{move(cb)}, err_cb{move(err_cb)}, queue{&queue} ]() {
        try {
          DataBlock data = slt::file::read(file);

          struct intermediate {
            ReadCallback cb_;
            DataBlock data_;

            void operator()() {
              cb_(std::move(data_));
            }
          };
          queue->queueEvent(intermediate{move(cb), std::move(data)});
          return true;
        } catch(ReadError &err) {
          struct intermediate {
            ReadFailedCallback cb_;
            ReadError err_;

            void operator()() {
              cb_(std::move(err_));
            }
          };
          queue->queueEvent(intermediate{move(err_cb), std::move(err)});
          return true;
        }
      });
}

void write(DataView data, StringView file) {
  // Do Not mess with this string, it's used in log parsing.
  slt::log->info("Writing to file: {}", file);

  std::ofstream out_file(file.toString(), std::ios::binary);

  if(!out_file.good()) {
    throw WriteError(fmt::format("Failed to write file: {}", file));
  }

  slt::log->info("Wrote {} bytes", data.size());

  out_file.write(data.data(), data.size());
}
}
}