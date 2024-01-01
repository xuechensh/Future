#include <thread>

namespace sh {

class ThreadWrapper {
 public:
  ThreadWrapper(std::thread&& rhf) : t_(std::move(rhf)) {}
  ~ThreadWrapper() {
    if (t_.joinable()) t_.join();
  }

 private:
  ThreadWrapper() = delete;
  ThreadWrapper(const ThreadWrapper& rhf) = delete;
  ThreadWrapper& operator=(const ThreadWrapper& rhf) = delete;

 private:
  std::thread t_;
};

}  // namespace sh
