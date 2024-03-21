#include "util/thread_pool.h"

#include <atomic>
#include <iostream>

std::atomic<uint64_t> count(0);

void test() {
  sh::ThreadPool thread_pool(50, 100);
  for (int i = 0; i < 100; i++) {
    thread_pool.Commit([]() { count.fetch_add(1, std::memory_order_relaxed); });
  }
}

int main() {
  test();
  std::cout << count.load(std::memory_order_relaxed) << std::endl;
  return 0;
}