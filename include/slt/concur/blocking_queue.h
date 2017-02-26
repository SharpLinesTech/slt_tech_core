#ifndef SLT_CONCUR_BLOCKING_QUEUE_H
#define SLT_CONCUR_BLOCKING_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace slt {
namespace concur {

// A simple mutli-threaded blocking queue.
template <typename T>
class BlockingQueue {
 public:
  // Block until something is available in the queue and return the front of
  // the queue.
  inline T pop();

  // Append an element to the queue.
  template <typename U>
  inline void push(U&& item);

  // Flushes the queue.
  inline void clear();

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
}
}

#include "slt/concur/details/blocking_queue_impl.h"

#endif