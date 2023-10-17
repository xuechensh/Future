#include <iostream>

namespace sh {

template <typename T>
class unique_ptr {
 public:
  explicit constexpr unique_ptr(T* ptr = nullptr) noexcept : ptr_(ptr) {}
  ~unique_ptr() noexcept {
    delete ptr_;
    ptr_ = nullptr;
  }

  unique_ptr(unique_ptr&& other) noexcept { ptr_ = other.release(); }

  template <typename U>
  unique_ptr(unique_ptr<U>&& other) noexcept {
    ptr_ = other.release();
  }

  T& operator*() const noexcept { return *ptr_; }
  T* operator->() const noexcept { return ptr_; }

  operator bool() const noexcept { return ptr_ != nullptr; }
  T* get() const noexcept { return ptr_; }

  T* release() noexcept {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void swap(unique_ptr& rhs) noexcept {
    using std::swap;
    swap(ptr_, rhs.ptr_);
  }

 private:
  T* ptr_;
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace sh

class A {
 public:
  A() { std::cout << "A" << std::endl; }
  virtual ~A() {}

  virtual void print() { std::cout << "A print" << std::endl; }
};

class B : public A {
 public:
  B() { std::cout << "B" << std::endl; }
  void print() override { std::cout << "B print" << std::endl; }
};

int main() {
  auto a = sh::make_unique<A>();
  a->print();

  sh::unique_ptr<A> aa(std::move(a));
  aa->print();

  auto b = sh::make_unique<B>();
  b->print();

  sh::unique_ptr<A> cc(std::move(b));
  cc->print();

  return 0;
}
