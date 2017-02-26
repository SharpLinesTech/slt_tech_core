#ifndef SLT_CONCURENCY_BLOCKING_QUEUE_H
#define SLT_CONCURENCY_BLOCKING_QUEUE_H

namespace slt {
namespace concur {

template <typename T>
T BlockingQueue<T>::pop() {
  std::unique_lock<std::mutex> lock(mutex_);
  while(queue_.empty()) {
    cond_.wait(lock);
  }
  auto item = std::move(queue_.front());
  queue_.pop();
  return item;
}

template <typename T>
template <typename U>
void BlockingQueue<T>::push(U&& item) {
  std::unique_lock<std::mutex> lock(mutex_);
  queue_.emplace(std::forward<U>(item));
  lock.unlock();
  cond_.notify_one();
}

template <typename T>
void BlockingQueue<T>::clear() {
  std::unique_lock<std::mutex> lock(mutex_);
  queue_ = std::queue<T>();
}
}
}

#endif