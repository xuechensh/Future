#include <iostream>
#include <memory>

enum ShapeType {
  kCircle = 0,
  kTriangle,
  kRectangle,
};

class Shape {
 public:
  Shape() { std::cout << "shape" << std::endl; }
  virtual ~Shape() { std::cout << "release shape" << std::endl; }

  virtual void print() = 0;
};

class Circle : public Shape {
 public:
  Circle() { std::cout << "Circle" << std::endl; }

  void print() override { std::cout << "I am Circle" << std::endl; }
};

class Triangle : public Shape {
 public:
  Triangle() { std::cout << "Triangle" << std::endl; }

  void print() override { std::cout << "I am Triangle" << std::endl; }
};

class Rectangle : public Shape {
 public:
  Rectangle() { std::cout << "Rectangle" << std::endl; }

  void print() override { std::cout << "I am Rectangle" << std::endl; }
};

std::unique_ptr<Shape> createShape(const ShapeType& type) {
  switch (type) {
    case kCircle:
      return std::make_unique<Circle>();
    case kTriangle:
      return std::make_unique<Triangle>();
    case kRectangle:
      return std::make_unique<Rectangle>();
    default:
      return nullptr;
  }
}

int main() {
  auto shape = createShape(kCircle);
  shape->print();
  return 0;
}
