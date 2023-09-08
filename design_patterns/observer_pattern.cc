/* 观察者模式
 * 当存在一对多的情况下，一个对象的状态的改变会通知其他对象
 */
#include <iostream>
#include <list>
#include <memory>
#include <string>

/* 观察者抽象父类 */
class Observer {
 public:
  explicit Observer(const std::string& name) : name_(name) {}
  virtual ~Observer(){};
  virtual void Update(const std::string& message) = 0;

 protected:
  std::string name_;
};

/* 被观察者，可以添加删除观察者，并实现通知函数 */
class Subject {
 public:
  virtual ~Subject(){};
  virtual void Attach(const std::shared_ptr<Observer>& observer) = 0;
  virtual void Detach(const std::shared_ptr<Observer>& observer) = 0;
  virtual void Notify(const std::string& message) = 0;
};

/* 具体被观察者派生类 */
class SubjectA : public Subject {
 public:
  ~SubjectA() { std::cout << "Exit SubjectA" << std::endl; }
  void Attach(const std::shared_ptr<Observer>& observer) override {
    for (auto iter = list_observer_.begin(); iter != list_observer_.end();
         ++iter) {
      if (*iter == observer) {
        std::cout << "Observer already exit in subject" << std::endl;
        return;
      }
    }
    list_observer_.emplace_back(observer);
    std::cout << "Observer attach success" << std::endl;
  }
  void Detach(const std::shared_ptr<Observer>& observer) override {
    for (auto iter = list_observer_.begin(); iter != list_observer_.end();
         ++iter) {
      if (*iter == observer) {
        list_observer_.erase(iter);
        std::cout << "Observer detach success" << std::endl;
        return;
      }
    }
    std::cout << "Observer not exit in subject" << std::endl;
  }
  void Notify(const std::string& message) override {
    std::cout << "Notify " << list_observer_.size() << " observers"
              << std::endl;
    for (auto iter = list_observer_.begin(); iter != list_observer_.end();
         ++iter) {
      (*iter)->Update(message);
    }
  }

 private:
  std::list<std::shared_ptr<Observer>> list_observer_;
};

/* 具体观察者类 */
class ObserverA : public Observer {
 public:
  ObserverA(const std::string& name) : Observer(name) {}
  void Update(const std::string& message) override {
    std::cout << "ObserverA, name " << name_ << " receive: " << message
              << std::endl;
  }
};

void Test() {
  auto observer1 = std::make_shared<ObserverA>("ob1");
  auto observer2 = std::make_shared<ObserverA>("ob2");
  auto subject = std::make_shared<SubjectA>();
  subject->Attach(observer1);
  subject->Attach(observer2);
  subject->Notify("First message");

  subject->Detach(observer2);
  subject->Notify("Second message");
}

int main() {
  Test();
  return 0;
}
