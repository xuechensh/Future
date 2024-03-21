#include "util/print.h"

#include <thread>
#include <vector>

void func(int thread_id) { sh::print("Thread", thread_id, "started"); }

void test01() {
  int max_i = 10000;
  std::vector<std::thread> thread_pool;
  for (int i = 0; i < max_i; ++i) {
    auto t = std::thread(func, i);
    thread_pool.emplace_back(std::move(t));
  }
  for (auto& t : thread_pool) {
    t.join();
  }
}

int main() {
  test01();
  return 0;
}