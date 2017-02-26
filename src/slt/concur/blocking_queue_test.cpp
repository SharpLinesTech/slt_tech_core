#include "gtest/gtest.h"

#include <chrono>

#include "slt/concur/blocking_queue.h"

TEST(BlockingQueue, SameThreadQueueAndDequeue) {
  slt::concur::BlockingQueue<int> values;

  values.push(1);
  values.push(2);
  values.push(3);
  values.push(4);
  EXPECT_EQ(1, values.pop());
  EXPECT_EQ(2, values.pop());
  EXPECT_EQ(3, values.pop());
  EXPECT_EQ(4, values.pop());
}

TEST(BlockingQueue, SimpleMultiThreaded) {
  slt::concur::BlockingQueue<int> values;

  std::thread worker([&values]() {
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(10ms);
    values.push(1);
    values.push(2);

    std::this_thread::sleep_for(10ms);
    values.push(3);
    values.push(4);
  });

  EXPECT_EQ(1, values.pop());
  EXPECT_EQ(2, values.pop());
  EXPECT_EQ(3, values.pop());
  EXPECT_EQ(4, values.pop());

  worker.join();
}