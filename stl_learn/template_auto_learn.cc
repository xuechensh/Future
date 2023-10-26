#include <iostream>
#include <type_traits>

// 这里有一点需要注意
// T&&这种格式是万能引用的格式，只有这种格式，其他任意格式都不行
// 比如const T&&、vector<T>&&都不行、const T&&是右值引用
template <typename T>
void my_print(T&& t) {
  std::cout << std::boolalpha << t << std::endl;
}

template <typename T, typename... Args>
void my_print(T&& t, Args&&... args) {
  std::cout << std::boolalpha << t << ',';
  my_print(std::forward<Args>(args)...);
}

void test01() { my_print(1, 2, "abcd", false); }

template <typename T, size_t N>
constexpr size_t arry_size(T (&)[N]) noexcept {
  return N;
}

void test02() {
  const int arr[3] = {1, 2, 3};
  my_print(arry_size(arr));
  my_print(arry_size("abcd"));
}

int a = 1;
const int b = 2;
int& c = a;
const int& d = b;
char e[] = "abc";
const char f[] = "abc";

void test_func(int a, int b) { my_print(a, b); }
void (&test_func_ref)(int, int) = test_func;
void (*test_func_ptr)(int, int) = test_func;

// 情景1：ParamType是一个指针或引用，但不是通用引用
// 这个相当于T可以携带const限定，也可以不带
template <typename T>
void func01(T& t) {
  if (std::is_same<T, int>::value && std::is_same<T&, int&>::value) {
    my_print("int -> int&");
  } else if (std::is_same<T, const int>::value &&
             std::is_same<T&, const int&>::value) {
    my_print("const int -> const int&");
  } else if (std::is_same<T, char[4]>::value &&
             std::is_same<T&, char(&)[4]>::value) {
    my_print("char[4] -> char(&)[4]");
  } else if (std::is_same<T, const char[4]>::value &&
             std::is_same<T&, const char(&)[4]>::value) {
    my_print("const char[4] -> const char(&)[4]");
  } else if (std::is_same<T, void(int, int)>::value &&
             std::is_same<T&, void (&)(int, int)>::value) {
    // 函数别名默认是const的，不允许修改，但函数指针可以修改
    my_print("void(int, int) -> void (&)(int, int)");
  } else {
    my_print("not find T");
  }
}

template <typename T>
void func02(const T& t) {
  if (std::is_same<T, int>::value &&
      std::is_same<const T&, const int&>::value) {
    my_print("int -> const int&");
  } else if (std::is_same<T, char[4]>::value &&
             std::is_same<const T&, const char(&)[4]>::value) {
    my_print("char[4] -> const char(&)[4]");
  } else if (std::is_same<T, void(int, int)>::value &&
             std::is_same<T&, void (&)(int, int)>::value) {
    // 函数别名默认是const的，不允许修改，但函数指针可以修改
    my_print("void(int, int) -> void (&)(int, int)");
  } else {
    my_print("not find T");
  }
}

void test03() {
  my_print("func01 <<<<<<<<<<<<<<<");
  func01(a);
  func01(b);
  func01(c);
  func01(d);
  func01(e);
  func01(f);
  // func01(1); // 编译失败，左值引用不能引用右值
  func01("abc");
  func01(test_func);
  func01(test_func_ref);

  my_print("func02 <<<<<<<<<<<<<<<");
  func02(a);
  func02(b);
  func02(c);
  func02(d);
  func02(e);
  func02(f);
  func02(1);  // const左值引用可以引用右值
  func02("abc");
  func02(test_func);
  func02(test_func_ref);
}

// 情景2：ParamType是一个通用引用（万能引用）
// 一开始是什么类型，ParamType还是什么类型，只不过如果是右值引用的话，
// T是原始类型，不是左值引用类型
template <typename T>
void func03(T&&) {
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
  } else if (std::is_same<T, char(&)[4]>::value &&
             std::is_same<T&&, char(&)[4]>::value) {
    my_print("char(&)[4] -> char(&)[4]");
  } else if (std::is_same<T, const char(&)[4]>::value &&
             std::is_same<T&&, const char(&)[4]>::value) {
    my_print("const char(&)[4] -> const char(&)[4]");
  } else if (std::is_same<T, char[4]>::value &&
             std::is_same<T&&, char(&&)[4]>::value) {
    my_print("char[4] -> char(&&)[4]");
  } else if (std::is_same<T, const char[4]>::value &&
             std::is_same<T&&, const char(&&)[4]>::value) {
    my_print("const char[4] -> const char(&&)[4]");
  } else if (std::is_same<T, void (&)(int, int)>::value &&
             std::is_same<T&, void (&)(int, int)>::value) {
    // 函数别名默认是const的，不允许修改，但函数指针可以修改
    my_print("void (&)(int, int) -> void (&)(int, int)");
  } else {
    my_print("not find T");
  }
}

void test04() {
  my_print("func03 <<<<<<<<<<<<<<<");
  func03(a);
  func03(b);
  func03(std::move(b));
  func03(c);
  func03(d);
  func03(e);
  func03(std::move(e));
  func03(f);
  func03(std::move(f));
  func03(1);  // const左值引用可以引用右值
  func03("abc");
  func03(test_func);
  func03(test_func_ref);
}

// 情景3：ParamType既不是指针也不是引用
// 会拷贝一个完整对象，在引用到数组的时候会退化成指针，在引用函数对象时退化成函数指针
// T之前可以加const限定，也可以不加const限定，这只表示param是否是const，而不是类型是否是const
// 不影响T的类型推导，但数组退化成指针，如果数组是const，T推导为const指针
template <typename T>
void func04(T t) {
  if (std::is_same<T, int>::value) {
    my_print("int");
  } else if (std::is_same<T, char*>::value) {
    my_print("char*");
  } else if (std::is_same<T, const char*>::value) {
    my_print("const char*");
  } else if (std::is_same<T, void (*)(int, int)>::value) {
    my_print("void(*)(int, int)");
  } else {
    my_print("not find T");
  }
}

template <typename T>
void func05(const T t) {
  if (std::is_same<const T, const int>::value) {
    my_print("int");
  } else if (std::is_same<const T, char* const>::value) {
    my_print("char* const");
  } else if (std::is_same<const T, const char* const>::value) {
    my_print("const char* const");
  } else if (std::is_same<const T, void (*const)(int, int)>::value) {
    my_print("void (*const)(int, int)");
  } else {
    my_print("not find T");
  }
}

void test05() {
  my_print("func04 <<<<<<<<<<<<<<<");
  func04(a);
  func04(b);
  func04(std::move(b));
  func04(c);
  func04(d);
  func04(e);
  func04(std::move(e));
  func04(f);
  func04(std::move(f));
  func04(1);
  func04("abc");
  func04(test_func);
  func04(test_func_ref);

  my_print("func05 <<<<<<<<<<<<<<<");
  func05(a);
  func05(b);
  func05(std::move(b));
  func05(c);
  func05(d);
  func05(e);
  func05(std::move(e));
  func05(f);
  func05(std::move(f));
  func05(1);
  func05("abc");
  func05(test_func);
  func05(test_func_ref);
}

// 附加4：cost对象move后资源情况
// const右值引用，资源是掠夺不了的！！！！
// 默认是不生成const&&的构造函数的，如果不自定义，则编译不过
class A {
 public:
  A() : a_(1) {}
  A(A&& rhf) : a_(rhf.a_) { rhf.a_ = 0; }
  A(const A&& rhf) : a_(rhf.a_) {}

  int a_;
};

void test06() {
  const A a;
  A b(std::move(a));
  my_print(a.a_, b.a_);
}

int main() {
  test01();
  test02();
  test03();
  test04();
  test05();
  test06();
  return 0;
}
