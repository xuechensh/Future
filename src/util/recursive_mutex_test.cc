#include "util/recursive_mutex.h"

#include <iostream>
#include <thread>
#include <vector>

static sh::RecursiveMutex mutex;

void TestLock(int thread_id, int level) {
  if (level == 0) return;

  mutex.lock();
  std::cout << thread_id << ": abcedfghijklmnopqrstuvwxyz" << std::endl;
  TestLock(thread_id, --level);
  mutex.unlock();
}

void test01() {
  std::vector<std::thread> thread_pool;
  for (int i = 0; i < 1000; ++i) {
    auto t = std::thread(TestLock, i, 5);
    thread_pool.emplace_back(std::move(t));
  }
  for (int i = 0; i < 1000; ++i) {
    thread_pool[i].join();
  }
}

void TestTryLock(int thread_id, int level) {
  if (level == 0) return;

  if (mutex.try_lock()) {
    std::cout << thread_id << ": abcedfghijklmnopqrstuvwxyz" << std::endl;
    TestTryLock(thread_id, --level);
    mutex.unlock();
    return;
  }

  mutex.lock();
  std::cout << thread_id << ": try lock failed" << std::endl;
  mutex.unlock();
  return;
}

void test02() {
  std::vector<std::thread> thread_pool;
  for (int i = 0; i < 1000; ++i) {
    auto t = std::thread(TestTryLock, i, 5);
    thread_pool.emplace_back(std::move(t));
  }
  for (int i = 0; i < 1000; ++i) {
    thread_pool[i].join();
  }
}

int main() {
  test01();
  test02();
  return 0;
}