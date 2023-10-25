#include <iostream>

#include "widget.h"

// 了解就行了，知道有这么用，不深究
// 目前理解是用在隐藏内部实现，减少头文件依赖，提高编译速度
int main() {
  Widget w1;
  Widget w2(w1);
  Widget w3(std::move(w1));

  return 0;
}
