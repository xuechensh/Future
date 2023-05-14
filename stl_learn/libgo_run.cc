#include "libgo/coroutine.h"
#include <iostream>

int main(int argc, char** argv) {
  std::cout << "hello" << std::endl;
  go[] {
    std::cout << "hello1" << std::endl;
    co_yield;
    std::cout << "hello2" << std::endl;
  };

  go[] {
    std::cout << "hello3" << std::endl;
    co_yield;
    std::cout << "hello4" << std::endl;
  };

  std::thread t([] { co_sched.Start(); });
  t.detach();
  co_sleep(100);

  return 0;
}
