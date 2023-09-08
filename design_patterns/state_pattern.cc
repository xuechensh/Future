/* 状态模式
 * 1、对象需要根据自身当前状态进行不同行为，同时状态的数量非常多且与状态相关的代码会频繁变更。
 * 2、某个类需要根据成员变量的当前值改变自身行为，从而需要使用大量的条件语句时，可使用状态模式。
 * 3、相似状态和基于条件的状态机转换中存在许多重复代码时，可使用状态模式。
 * 核心点在于一个类的业务逻辑和当前所处状态有着十分紧密的联系，逻辑的运行依赖状态的转换！！
 */
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

class Context;

class State {
 public:
  explicit State(std::unique_ptr<std::string>&& doc) : doc_(std::move(doc)) {}
  virtual ~State() = default;

  virtual bool Write(const std::string& message) { return false; }
  virtual std::string GetDoc() const { return {}; }

  virtual std::unique_ptr<State> Submit() { return nullptr; }
  virtual std::unique_ptr<State> Reject() { return nullptr; }

 protected:
  std::unique_ptr<std::string> doc_;
};

class StateDraft : public State {
 public:
  explicit StateDraft(std::unique_ptr<std::string>&& doc)
      : State(std::move(doc)) {}
  bool Write(const std::string& message) override;
  std::unique_ptr<State> Submit() override;
};

class StateModeration : public State {
 public:
  explicit StateModeration(std::unique_ptr<std::string>&& doc)
      : State(std::move(doc)) {}
  std::unique_ptr<State> Submit() override;
  std::unique_ptr<State> Reject() override;
};

class StatePublished : public State {
 public:
  explicit StatePublished(std::unique_ptr<std::string>&& doc)
      : State(std::move(doc)) {}
  std::string GetDoc() const override;
  std::unique_ptr<State> Reject() override;
};

bool StateDraft::Write(const std::string& message) {
  *doc_ += message;
  return true;
}
std::unique_ptr<State> StateDraft::Submit() {
  return std::make_unique<StateModeration>(std::move(doc_));
}

std::unique_ptr<State> StateModeration::Submit() {
  return std::make_unique<StatePublished>(std::move(doc_));
}
std::unique_ptr<State> StateModeration::Reject() {
  return std::make_unique<StateDraft>(std::move(doc_));
}

std::string StatePublished::GetDoc() const { return *doc_; }
std::unique_ptr<State> StatePublished::Reject() {
  return std::make_unique<StateDraft>(std::move(doc_));
}

class Context {
 public:
  Context()
      : state_(std::make_unique<StateDraft>(std::make_unique<std::string>())) {}

  void Write(const std::string& message) {
    if (state_->Write(message)) {
      std::cout << "Write success" << std::endl;
    } else {
      std::cout << "Curr state not support Write" << std::endl;
    }
  }

  void Read() {
    auto doc = state_->GetDoc();
    if (!doc.empty()) {
      std::cout << doc << std::endl;
    } else {
      std::cout << "Curr state not support Read" << std::endl;
    }
  }

  void Submit() {
    auto state = state_->Submit();
    if (!state) {
      std::cout << "Curr state not support Submit" << std::endl;
      return;
    }
    state_ = std::move(state);
  }

  void Reject() {
    auto state = state_->Reject();
    if (!state) {
      std::cout << "Curr state not support Reject" << std::endl;
      return;
    }
    state_ = std::move(state);
  }

 private:
  std::unique_ptr<State> state_;
};

void Test() {
  auto context = std::make_unique<Context>();
  context->Write("abcde");
  context->Submit();
  context->Read();
  context->Reject();
  context->Submit();
  context->Submit();
  context->Write("edfg");
  context->Read();
}

int main() {
  Test();
  return 0;
}
