#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "util/bounded_queue.h"

namespace sh {

template <typename T>
class BoundedQueue {
 public:
  BoundedQueue() = default;
  ~BoundedQueue();

  BoundedQueue(const BoundedQueue& rhf) = delete;
  BoundedQueue& operator=(const BoundedQueue& rhf) = delete;

  BoundedQueue(BoundedQueue&& rhf) = delete;
  BoundedQueue& operator=(BoundedQueue&& rhf) = delete;

  bool Init(const uint64_t& size);

  bool Enqueue(const T& element);
  std::unique_ptr<T> Dequeue();

  bool empty() const;
  bool full() const;

 private:
  std::vector<std::unique_ptr<T*>> pool_;
  uint64_t capacity_;

  uint64_t head_ = 0;
  uint64_t tail_ = 0;
};

template <typename T>
BoundedQueue<T>::~BoundedQueue() {
  for (auto& element : pool_) {
    auto old_element = element.exchange(nullptr, std::memory_order_acq_rel);
    delete old_element;
  }
}

template <typename T>
bool BoundedQueue<T>::Init(const uint64_t& size) {
  try {
    pool_.resize(size + 1, nullptr);
  } catch (const std::exception& ex) {
    std::cout << ex.what << std::endl;
    return false;
  }
  capacity_ = size + 1;
  return true;
}

template <typename T>
bool BoundedQueue<T>::Enqueue(const T& element) {
  if (full()) return false;

  T* inner_element = nullptr;
  try {
    inner_element = new T(element);
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return false;
  }

  auto old_tail = tail_.load(std::memory_order_acquire);
  T* target = nullptr;
  while (!pool_[old_tail].compare_exchange_weak(target, inner_element,
                                                std::memory_order_acq_rel,
                                                std::memory_order_acquire)) {
    auto old_tail = tail_.load(std::memory_order_acquire);
  }

  while (!tail_.compare_exchange_weak(old_tail, (old_tail + 1) % capacity_,
                                      std::memory_order_acq_rel,
                                      std::memory_order_acquire)) {}
}

template <typename T>
std::unique_ptr<T> BoundedQueue<T>::Dequeue() {
  auto tail = tail_.load(std::memory_order_acquire);
}

template <typename T>
inline bool BoundedQueue<T>::empty() const {
  auto head = head_.load(std::memory_order_acquire);
  auto tail = tail_.load(std::memory_order_acquire);

  return head == tail;
}

template <typename T>
inline bool BoundedQueue<T>::full() const {
  auto head = head_.load(std::memory_order_acquire);
  auto tail = tail_.load(std::memory_order_acquire);

  return head == (tail + 1) % capacity_;
}

}  // namespace sh
