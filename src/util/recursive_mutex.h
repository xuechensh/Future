#include <atomic>
#include <thread>

namespace sh {

class RecursiveMutex {
 public:
  void lock() {
    auto this_thread_id = std::this_thread::get_id();
    if (this_thread_id == thread_id_.load(std::memory_order_acquire)) {
      ++count_;
      return;
    }

    std::thread::id expect_thread_id;
    while (!thread_id_.compare_exchange_weak(expect_thread_id, this_thread_id,
                                             std::memory_order_release,
                                             std::memory_order_acquire)) {
      asm volatile("rep; nop" ::: "memory");
      expect_thread_id = std::thread::id();
    }
    ++count_;
  }

  bool try_lock() {
    auto this_thread_id = std::this_thread::get_id();
    if (this_thread_id == thread_id_.load(std::memory_order_acquire)) {
      ++count_;
      return true;
    }

    std::thread::id expect_thread_id;
    if (!thread_id_.compare_exchange_weak(expect_thread_id, this_thread_id,
                                          std::memory_order_release,
                                          std::memory_order_acquire)) {
      return false;
    }
    ++count_;
    return true;
  }

  void unlock() {
    auto this_thread_id = std::this_thread::get_id();
    if (this_thread_id != thread_id_.load(std::memory_order_acquire)) {
      return;
    }

    if (--count_ == 0) {
      thread_id_.store(std::thread::id(), std::memory_order_release);
    }
    return;
  }

 private:
  std::atomic<std::thread::id> thread_id_;
  uint64_t count_ = 0;
};

}  // namespace sh