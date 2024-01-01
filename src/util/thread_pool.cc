#include "util/thread_pool.h"

namespace sh {

ThreadPool::ThreadPool(const std::size_t& thread_num,
                       const std::size_t& max_task_num)
    : shutdown_(false) {
  for (std::size_t i = 0; i < thread_num; ++i) {
    pool_.emplace_back(&ThreadPool::Schedual, this);
  }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard<AtomicMutex> lock(mutex_);
    shutdown_ = true;
  }
  for (auto& t : pool_) {
    t.join();
  }
}

ThreadPool::TaskState ThreadPool::GetTask(ThreadPool::Task* task) {
  std::unique_lock<AtomicMutex> lock(mutex_);
  if (shutdown_) return kShutdown;
  if (tasks_.empty()) return kWaiting;
  *task = std::move(tasks_.front());
  tasks_.pop();
  return kSuccess;
}

void ThreadPool::Schedual() {
  Task task;
  while (true) {
    auto state = GetTask(&task);
    if (state == kShutdown) {
      break;
    } else if (state == kWaiting) {
      asm volatile("rep; nop" ::: "memory");
      continue;
    } else if (state == kSuccess) {
      task();
      continue;
    } else {
      break;
    }
  }
}

}  // namespace sh