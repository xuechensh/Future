#include <iostream>
#include <memory>
#include <string>

class Strategy {
 public:
  virtual ~Strategy() = default;
  virtual std::string Read(const int32_t& index) const = 0;
};

class StrategyA : public Strategy {
 public:
  std::string Read(const int32_t& index) const override {
    return "StrategyA read " + std::to_string(index);
  }
};

class StrategyB : public Strategy {
 public:
  std::string Read(const int32_t& index) const override {
    return "StrategyB read " + std::to_string(index);
  }
};

class Context {
 public:
  Context() : strategy_(std::make_unique<StrategyA>()) {}
  Context(std::unique_ptr<Strategy>&& Strategy)
      : strategy_(std::move(Strategy)) {}
  void Reset(std::unique_ptr<Strategy>&& strategy) {
    strategy_ = std::move(strategy);
  }

  void DoBusiness(const int32_t&& index) {
    std::cout << "Context DoBusiness:" << std::endl;
    std::cout << strategy_->Read(index) << std::endl;
  }

 private:
  std::unique_ptr<Strategy> strategy_;
};

void Test() {
  auto context = std::make_unique<Context>();
  context->DoBusiness(1);

  auto strategy_b = std::make_unique<StrategyB>();
  context->Reset(std::move(strategy_b));
  context->DoBusiness(2);
}

int main() {
  Test();
  return 0;
}