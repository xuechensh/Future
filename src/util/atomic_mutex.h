#include <atomic>

namespace sh {

class AtomicMutex {
 public:
  void lock() {
    while (mutex_.test_and_set(std::memory_order_acquire)) {
      asm volatile("rep; nop" ::: "memory");
    }
  }

  bool try_lock() {
    if (mutex_.test_and_set(std::memory_order_acquire)) {
      return false;
    } else {
      return true;
    }
  }

  void unlock() { mutex_.clear(std::memory_order_release); }

 private:
  std::atomic_flag mutex_ = ATOMIC_FLAG_INIT;
};

}  // namespace sh
