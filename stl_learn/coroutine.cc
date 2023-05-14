#include <boost/coroutine2/all.hpp>
#include <boost/fiber/all.hpp>
#include <iostream>
#include <string>

void coroutine_function(
    boost::coroutines2::coroutine<void>::pull_type& coro_back) {
  std::cout << "a " << std::endl;
  coro_back(); // 锚点，返回
  std::cout << "b " << std::endl;
  coro_back(); // 锚点 返回
  std::cout << "c " << std::endl;
}

void test01() {
  boost::coroutines2::coroutine<void>::push_type coroutine_object(
      coroutine_function); // 创建协程
  std::cout << "1 " << std::endl;
  coroutine_object(); // 运行协程
  std::cout << "2 " << std::endl;
  coroutine_object(); // 返回锚点，继续运行协程
  std::cout << "3 " << std::endl;
  coroutine_object(); // 返回锚点，继续运行协程
}

void foo(boost::coroutines2::coroutine<int>::push_type& sink) {
  std::cout << "start coroutine\n";
  sink(1);
  std::cout << "finish coroutine\n";
  sink(2);
}

void test02() {
  boost::coroutines2::coroutine<int>::pull_type source(foo);
  std::cout << source.get() << std::endl;
  std::cout << source().get() << std::endl;
  std::cout << "finish\n";
}

void input(boost::coroutines2::coroutine<int>::pull_type& sink) {
  std::cout << "start input" << std::endl;
  int a = sink().get();
  std::cout << "Num: " << a << std::endl;
  std::cout << "finish input" << std::endl;
}

void test03() {
  std::cout << "init test03" << std::endl;
  boost::coroutines2::coroutine<int>::push_type source(input);

  std::cout << "start test03" << std::endl;
  source(1);
  std::cout << "test03 inner" << std::endl;
  source(5); // not use
  std::cout << "finish test03" << std::endl;
}

void call_me(boost::fibers::buffered_channel<std::string>& pipe) {
  std::cout << "start call me" << std::endl;
  pipe.push("haha");
}

void test04() {
  boost::fibers::buffered_channel<std::string> pip(2);
  boost::fibers::fiber f([&]() { call_me(pip); });
  f.detach();

  std::string str;
  std::cout << "start pop" << std::endl;
  pip.pop(str);
  std::cout << "get str: " << str << std::endl;
  return;
}

int main() {
  test01();
  test02();
  test03();
  test04();
  return 0;
}
