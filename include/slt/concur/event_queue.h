#ifndef SLT_CONCUR_EVENT_QUEUE_H
#define SLT_CONCUR_EVENT_QUEUE_H

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

namespace slt {
namespace concur {

// This class holds a FIFO queue of handlers to execute. Events can be queued or
// executed from any thread.
class EventQueue {
 public:
  using Event = std::function<void(void)>;

  // Queues a new event.
  void queueEvent(Event);

  // Executes all queued events.
  // Returns the number of consumed events.
  // If there is no event in the queue, this will return immediately.
  unsigned int executeAll();

  // Executes all queued events until predicate returns true. By default, the
  // predicate is checked only when the queue is fully empty. 
  //
  // Returns: 
  //   the number of consumed events.
  // 
  // Args:
  //   pred: The predicate to check to determine if we should abort
  //   batch_size: (optional) Check the predicate after this many events have
  //               been processed, even if there are events left in the queue.
  template <typename PRED_T>
  inline unsigned int executeUntil(PRED_T pred, unsigned int batch_size = 0);

 private:
  std::queue<Event> queue_;
  std::mutex mutex_;
  std::condition_variable condition_;
};
}
}

#include "slt/concur/details/event_queue_impl.h"

#endif