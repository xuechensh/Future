#include "util/atomic_mutex.h"

#include <gtest/gtest.h>

#include <thread>
#include <vector>

TEST(TestAtomicMutex, TestLockUnlock) {
  sh::AtomicMutex mtx;
  std::vector<std::thread> thread_pool;
  uint64_t num = 0;
  for (int i = 0; i < 100; i++) {
    thread_pool.emplace_back([&mtx, &num]() {
      mtx.lock();
      ++num;
      mtx.unlock();
    });
  }
  for (auto& t : thread_pool) {
    t.join();
  }
  EXPECT_EQ(num, 100);
}