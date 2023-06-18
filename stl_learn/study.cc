#include <chrono>
#include <iostream>
#include <type_traits>
#include <vector>

// 可变参数模板
// case1
template <typename T> void print(T&& t) { std::cout << t << std::endl; }
template <typename T, typename... Args> // Args 模板参数包, 0-1个参数
void print(T&& t, Args&&... args) {
  // T&&+模板可以匹配任意类型，但const不行
  // Args... 参数包展开，args函数参数包
  std::cout << t << ',';
  // args...函数参数包展开
  print(std::forward<Args>(args)...);
}

void test01() { print('a', 1, 1.5, "hahah"); }

// case2 通过函数模板计算某块代码的调用时长，代码块通过lambda传递
template <typename F, typename... Args>
std::invoke_result_t<F, Args...> runTimeWithReturn(F&& f, Args&&... args) {
  using ResultType = std::invoke_result_t<F, Args...>;
  auto start = std::chrono::high_resolution_clock::now();
  ResultType result = f(std::forward<Args>(args)...);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "runTime: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << std::endl;
  return result;
}

template <typename F, typename... Args>
void runTimeNoReturn(F&& f, Args&&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  f(std::forward<Args>(args)...);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "runTime: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << std::endl;
  return;
}

template <typename F, typename... Args>
std::invoke_result_t<F, Args...> runTime(F&& f, Args&&... args) {
  using ResultType = std::invoke_result_t<F, Args...>;
  if constexpr (std::is_void_v<ResultType>) {
    return runTimeNoReturn(std::forward<F>(f), std::forward<Args>(args)...);
  } else {
    return runTimeWithReturn(std::forward<F>(f), std::forward<Args>(args)...);
  }
}

void test02() {
  std::vector<int64_t> a;
  int sum = runTime([&]() {
    int sum = 0;
    for (int64_t i = 0; i < 10000; i++) {
      a.emplace_back(i);
      sum += i;
    }
    return sum;
  });
  std::cout << "sum: " << sum << std::endl;

  std::vector<int64_t> b;
  runTime([&]() {
    for (int64_t i = 0; i < 10000; i++) {
      b.emplace_back(i);
    }
  });
}

int main() {
  test01();
  test02();
  return 0;
}