#include <iostream>
#include <memory>
#include <string>

class State {
 public:
  explicit State(std::unique_ptr<std::string>&& doc) : doc_(std::move(doc)) {}

  virtual std::string Read() const { return {}; }
  virtual bool Write(const std::string& message) { return false; }

 private:
  std::unique_ptr<std::string> doc_;
};

class StateDraft : public State {
 public:
  explicit StateDraft(std::unique_ptr<std::string>&& doc)
      : State(std::move(doc)) {}
};
