#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "util/atomic_mutex.h"

namespace sh {

class ThreadPool {
 public:
  using Task = std::function<void()>;
  enum TaskState { kShutdown = 0, kWaiting, kSuccess };

 public:
  ThreadPool(const std::size_t& thread_num, const std::size_t& max_task_num);
  ~ThreadPool();

  template <typename F, typename... Args>
  auto Commit(F&& f, Args&&... args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    using ResType = std::invoke_result_t<F, Args...>;
    auto task = std::make_shared<std::packaged_task<ResType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    {
      std::lock_guard<AtomicMutex> lock(mutex_);
      if (shutdown_) {
        return std::future<ResType>();
      }
      tasks_.emplace([task = task]() { (*task)(); });
    }

    return task->get_future();
  }

 private:
  TaskState GetTask(Task* task);
  void Schedual();

 private:
  std::vector<std::thread> pool_;
  std::queue<Task> tasks_;
  AtomicMutex mutex_;
  bool shutdown_;
};

}  // namespace sh
