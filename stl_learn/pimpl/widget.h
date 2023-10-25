#include <memory>

// 这种写法只适用于unique_ptr
class Widget {
 public:
  Widget();

  // 除构造函数，其他默认函数要在头文件中生成，在.cc中定义
  ~Widget();

  Widget(const Widget& rhs);
  Widget(Widget&& rhs);

  Widget& operator=(const Widget& rhs);
  Widget& operator=(Widget&& rhs);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
