#include <chrono>

#include "src/atomic_mutex.h"

namespace sh {

class SnowFlake {
 public:
  explicit SnowFlake(const uint64_t& machine_id)
      : sequence_id_(0),
        machine_id_(machine_id),
        last_timestamp_(0),
        valid_(true) {
    if (machine_id_ > max_machine_num_) valid_ = false;
  }

  uint64_t NextId() {
    if (!valid_) throw std::runtime_error("SnowFlake invalid");

    std::lock_guard<AtomicMutex> lock(mtx_);

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();
    if (now < last_timestamp_) throw std::runtime_error("timestamp error");

    if (now == last_timestamp_) {
      sequence_id_ = (sequence_id_ + 1) & max_sequence_num_;
      if (sequence_id_ == 0) {
        now = GetNextMill();
      }
    } else {
      sequence_id_ = 0;
    }
    last_timestamp_ = now;

    auto ret = (now - start_time_stamp_) << timestamp_left_ |
               (machine_id_ << machine_left_) | sequence_id_;
    return ret;
  }

 private:
  uint64_t GetNextMill() {
    uint64_t now = 0;
    do {
      now = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    } while (now <= last_timestamp_);
    return now;
  }

 private:
  // 位宽
  static const uint64_t start_time_stamp_ = 1480166465631;
  static const uint64_t sequence_bit_ = 12;
  static const uint64_t machine_bit_ = 10;
  static const uint64_t timestamp_bit_ = 41;

  // 最大值
  static const uint64_t max_sequence_num_ =
      -1 ^ (uint64_t(-1) << sequence_bit_);
  static const uint64_t max_machine_num_ = -1 ^ (uint64_t(-1) << machine_bit_);
  static const uint64_t max_timestamp_ = -1 ^ (uint64_t(-1) << timestamp_bit_);

  // 下标
  static const uint64_t sequence_left_ = 0;
  static const uint64_t machine_left_ = sequence_bit_;
  static const uint64_t timestamp_left_ = sequence_bit_ + machine_bit_;

  uint64_t sequence_id_;
  uint64_t machine_id_;
  uint64_t last_timestamp_;
  bool valid_;

  AtomicMutex mtx_;
};

}  // namespace sh
