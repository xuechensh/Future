#include <iostream>
#include <mutex>

#include "util/recursive_mutex.h"

namespace sh {

extern RecursiveMutex mutex;

template <typename T>
void print(T&& t) {
  std::lock_guard<RecursiveMutex> lock(mutex);
  std::cout << t << std::endl;
}

template <typename T, typename... Args>
void print(T&& t, Args&&... args) {
  std::lock_guard<RecursiveMutex> lock(mutex);
  std::cout << t;
  print(std::forward<Args>(args)...);
}

}  // namespace sh
