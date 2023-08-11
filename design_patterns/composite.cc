#include <iostream>
#include <list>
#include <memory>
#include <string>

class Component {
 public:
  virtual ~Component() = default;
  virtual void Add(const std::shared_ptr<Component>& component){};
  virtual void Remove(const std::shared_ptr<Component>& component){};

  virtual bool IsComposite() const { return false; }
  virtual std::string Operator() const = 0;
};

class Leaf : public Component {
 public:
  explicit Leaf(const std::string& name) : name_(name) {}
  std::string Operator() const override { return std::string("Leaf_") + name_; }

 private:
  using Component::Add;
  using Component::Remove;

 private:
  std::string name_;
};

class Composite : public Component {
 public:
  explicit Composite(const std::string& name) : name_(name) {}

  void Add(const std::shared_ptr<Component>& component) override {
    components_.emplace_back(component);
  }
  void Remove(const std::shared_ptr<Component>& component) override {
    components_.remove(component);
  }

  bool IsComposite() const override { return true; }
  std::string Operator() const override {
    std::string ret;
    for (const auto& component : components_) {
      ret += component->Operator() + std::string(",");
    }
    if (!ret.empty()) ret.erase(ret.end() - 1);
    return "[Composite " + name_ + ": " + ret + "]";
  }

 private:
  std::string name_;
  std::list<std::shared_ptr<Component>> components_;
};

int main() {
  auto leaf1 = std::make_shared<Leaf>("1");
  auto leaf2 = std::make_shared<Leaf>("2");
  auto leaf3 = std::make_shared<Leaf>("3");

  auto composite1 = std::make_shared<Composite>("1");
  auto composite2 = std::make_shared<Composite>("2");

  composite1->Add(leaf1);
  composite1->Add(leaf2);
  composite2->Add(leaf3);

  auto composite3 = std::make_shared<Composite>("3");
  composite3->Add(composite1);
  composite3->Add(composite2);
  std::cout << composite3->Operator() << std::endl;

  composite1->Remove(leaf1);
  std::cout << composite3->Operator() << std::endl;
}
