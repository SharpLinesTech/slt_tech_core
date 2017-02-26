#include "gtest/gtest.h"

#include "slt/concur/event_queue.h"

TEST(EventQueue, QueueAndExecute) {
  slt::concur::EventQueue queue;

  int value = 0;
  queue.queueEvent([&]() { value += 1; });
  queue.queueEvent([&]() { value += 2; });
  queue.queueEvent([&]() { value += 3; });
  queue.queueEvent([&]() { value += 4; });
  queue.queueEvent([&]() { value += 5; });

  // Nothing should have run by now.
  EXPECT_EQ(value, 0);
  queue.executeAll();
  EXPECT_EQ(value, 15);
}

TEST(EventQueue, ExecuteUntil) {
  slt::concur::EventQueue queue;

  int value = 0;
  for(int i = 0; i < 100; ++i) {
    queue.queueEvent([&]() { value += 1; });
  }

  // Nothing should have run by now.
  EXPECT_EQ(value, 0);

  queue.executeUntil([&value]() { return value > 50; }, 5);

  // with a batch size of 5, we should have executed 55 events.
  EXPECT_GE(value, 55);
}

TEST(EventQueue, ExecuteUntilAborted) {
  slt::concur::EventQueue queue;

  int value = 0;
  for(int i = 0; i < 52; ++i) {
    queue.queueEvent([&]() { value += 1; });
  }

  // Nothing should have run by now.
  EXPECT_EQ(value, 0);

  queue.executeUntil([&value]() { return value > 50; }, 5);

  // with a batch size of 5, but there are only 52 events in the queue...
  EXPECT_GE(value, 52);
}

TEST(EventQueue, ExecuteUntil_Threaded) {
  slt::concur::EventQueue queue;

  int value = 0;

  std::thread worker([&value, &queue]() {
    using namespace std::chrono_literals;
    for(int i = 0; i < 100; ++i) {
      std::this_thread::sleep_for(1ms);
      queue.queueEvent([&]() { value += 1; });
    }
  });

  // Nothing should have run by now.
  EXPECT_EQ(value, 0);

  queue.executeUntil([&value]() { return value >= 50; });
  EXPECT_GE(value, 50);

  worker.join();
}
