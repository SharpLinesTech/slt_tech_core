#ifndef SLT_CONCUR_EVENT_QUEUE_IMPL_H
#define SLT_CONCUR_EVENT_QUEUE_IMPL_H

namespace slt {
namespace concur {
template <typename PRED_T>
unsigned int EventQueue::executeUntil(PRED_T pred, unsigned int batch_size) {
  unsigned int executed_event_count = 0;
  unsigned int current_batch_count = 0;
  while(true) {
    std::unique_lock<std::mutex> lock(mutex_);
    if(queue_.empty()) {
      // Always check the predicate when we hit an empty queue.
      if(pred()) {
        break;
      } else {
        // The queue is empty and the predicate returned false, we can just
        // block until somethign goes in the queue.
        condition_.wait(lock);
        continue;
      }
    }

    // The order of events is important here:
    // Dequeue -> Unlock -> Invoke
    // This is because an event can enqueue a new event.
    Event cb = move(queue_.front());
    queue_.pop();
    lock.unlock();
    cb();

    ++executed_event_count;

    // Check to see if it's time to check the predicate.
    ++current_batch_count;
    if(batch_size != 0 && current_batch_count >= batch_size) {
      if(pred()) {
        break;
      } else {
        current_batch_count = 0;
      }
    }
  }

  return executed_event_count;
}
}
}

#endif