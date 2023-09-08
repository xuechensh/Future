#include <iostream>
#include <memory>

/* 模板模式将算法分解为一系列步骤，然后将步骤改写为方法，最后在模板方法中调用这些方法
 * 因此需要对共有步骤抽象，然后通过虚函数的方式调用实际的子类方法
 * 相比于策略模式，模板方法是静态的，不能在运行的时候切换行为
 * 可以理解为子类对父类公共步骤的共享，而策略模式是一种组合，是相同逻辑调用不同底层模块
 * 主要应用于：扩展某个特定的算法步骤，而不是整个算法结构；但当父类算法有改变的时候要修改所有派生类
 */

class TemplateClass {
 public:
  void TemplateMethod() const {
    Operation1();
    Operation3();
    Operation2();
    Hook1();
  }

 protected:
  virtual void Operation3() const = 0;
  virtual void Hook1() const { std::cout << "Hook1" << std::endl; }

 private:
  void Operation1() const { std::cout << "Operation1" << std::endl; }
  void Operation2() const { std::cout << "Operation2" << std::endl; }
};

class ConcreteClass1 : public TemplateClass {
 private:
  void Operation3() const override {
    std::cout << "ConcreteClass1 Operation3" << std::endl;
  }
};

class ConcreteClass2 : public TemplateClass {
 private:
  void Operation3() const override {
    std::cout << "ConcreteClass2 Operation3" << std::endl;
  }

  void Hook1() const override {
    std::cout << "ConcreteClass2 Hook1" << std::endl;
  }
};

void Test() {
  auto business1 = std::make_unique<ConcreteClass1>();
  business1->TemplateMethod();

  auto business2 = std::make_unique<ConcreteClass2>();
  business2->TemplateMethod();
}

int main() {
  Test();
  return 0;
}
