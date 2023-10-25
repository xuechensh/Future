#include "widget.h"

#include <string>

// 头文件不在依赖gadget.h，当gadget.h有变化时减少编译时间。
#include "gadget.h"

struct Widget::Impl {
  std::string a, b;
  Gadget gadget_;
};

Widget::Widget() : impl_(std::make_unique<Impl>()) {
  impl_->a = "haha";
  impl_->b = "hehe";
  impl_->gadget_.a_ = 1;
  impl_->gadget_.b_ = 2;
}

Widget::~Widget() = default;

Widget::Widget(const Widget& rhs) : impl_(std::make_unique<Impl>(*rhs.impl_)) {}

Widget::Widget(Widget&& rhs) = default;

Widget& Widget::operator=(const Widget& rhs) {
  if (this == &rhs) return *this;
  *impl_ = *rhs.impl_;
  return *this;
}

Widget& Widget::operator=(Widget&& rhs) = default;
