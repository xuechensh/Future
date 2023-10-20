#include <iostream>
#include <type_traits>

// 这里有一点需要注意
// T&&这种格式是万能引用的格式，只有这种格式，其他任意格式都不行
// 比如const T&&、vector<T>&&都不行、const T&&是右值引用
template <typename T>
void my_print(T&& a) {
  std::cout << a << std::endl;
}

template <typename T, typename... Args>
void my_print(T&& a, Args&&... args) {
  std::cout << a << ',';
  my_print(std::forward<Args>(args)...);
}

// 获取数组大小
template <typename T, size_t N>
constexpr size_t arrySize(T (&)[N]) noexcept {
  return N;
}

// 以下所有情景，在函数参数类型里没有限定const T的时候
// T是否携带const受形参影响

// 情景1：ParamType是一个指针或引用，但不是通用引用
// 这个相当于T可以携带const限定，也可以不带
template <typename T>
void f0(T& param) {
  if (std::is_same<T, int>::value && std::is_same<T&, int&>::value) {
    my_print("int -> int&");
  } else if (std::is_same<T, const int>::value &&
             std::is_same<T&, const int&>::value) {
    my_print("const int -> const int&");
  } else if (std::is_same<T, const char[5]>::value &&
             std::is_same<T&, const char(&)[5]>::value) {
    my_print("const char[5] -> const char&[5]");
  } else {
    my_print("other T");
  }
}

// 规定以const的方式使用param
template <typename T>
void f1(const T& param) {
  if (std::is_same<T, int>::value &&
      std::is_same<const T&, const int&>::value) {
    my_print("int -> const int&");
  } else if (std::is_same<T, char[5]>::value &&
             std::is_same<const T&, const char(&)[5]>::value) {
    my_print("char[5] -> const char&[5]");
  } else {
    my_print("other T");
  }
}

// 情景2：ParamType是一个通用引用
template <typename T>
void f2(T&& param) {
  if (std::is_same<T, int&>::value && std::is_same<T&&, int&>::value) {
    my_print("int& -> int&");
  } else if (std::is_same<T, const int&>::value &&
             std::is_same<T&&, const int&>::value) {
    my_print("const int& -> const int&");
  } else if (std::is_same<T, int>::value && std::is_same<T&&, int&&>::value) {
    my_print("int -> int&&");
  } else if (std::is_same<T, const int>::value &&
             std::is_same<T&&, const int&&>::value) {
    my_print("const int -> const int&&");
  } else if (std::is_same<T, const char(&)[5]>::value &&
             std::is_same<T&&, const char(&)[5]>::value) {
    my_print("const char&[5] -> const char&[5]");
  } else {
    my_print("other T");
  }
}

// 情景3：ParamType既不是指针也不是引用
// 会拷贝一个完整对象，在引用到数组的时候会退化成指针
// T之前可以加const限定，也可以不加const限定，这只表示param是否是const
// 不影响T的类型推导，但数组退化成指针，如果数组是const，T推导为const指针
template <typename T>
void f3(T param) {
  if (std::is_same<T, int>::value) {
    my_print("int -> int");
  } else if (std::is_same<T, const char*>::value) {
    my_print("const char* -> const char*");
  } else {
    my_print("other T");
  }
}

// 情景4：函数实参，函数对象的引用默认是const的，加了const限定，不影响类型推导
void testFunc(int a, int b) { my_print("testFunc"); }

template <typename T>
void f4(T& func) {
  if (std::is_same<T, void(int, int)>::value &&
      std::is_same<T&, void (&)(int, int)>::value) {
    my_print("void(int, int) -> void(&)(int, int)");
  } else {
    my_print("other T");
  }
}

template <typename T>
void f5(T func) {
  if (std::is_same<T, void (*)(int, int)>::value) {
    my_print("void(*)(int, int) -> void(*)(int, int)");
  } else {
    my_print("other T");
  }
}

int main() {
  // 情景1：ParamType是一个指针或引用，但不是通用引用
  int a = 1;
  const int b = a;
  const int& c = a;
  const char abcd[] = "abcd";
  f0(a);
  f0(b);
  f0(c);
  f0(abcd);
  // f0(1); // 编译不通过，不能将左值引用绑定右值
  f1(a);
  f1(b);
  f1(c);
  f1(1);  // 编译通过，可以将const左值引用绑定右值
  f1(abcd);

  // 情景2：ParamType是一个通用引用
  my_print("<<<<<<<<<<<<<<<");
  f2(1);
  f2(a);
  f2(b);
  f2(c);
  f2(std::move(a));
  f2(abcd);

  // 情景3：ParamType既不是指针也不是引用
  my_print("<<<<<<<<<<<<<<<<");
  f3(1);
  f3(a);
  f3(b);
  f3(c);
  f3(abcd);

  // 测试获取数组长度
  my_print("<<<<<<<<<<<<<<<<");
  my_print(arrySize(abcd));

  // 情景4：函数实参，不考虑在函数参数加const，没这么用的
  my_print("<<<<<<<<<<<<<<<<");
  f4(testFunc);
  f5(testFunc);
  void (*const ptr)(int, int) = testFunc;
  f4(*ptr);
  f5(ptr);

  // 测试const对象是可以move的
  const int cc = 0;
  f2(std::move(cc));

  return 0;
}
