#include <iostream>
#include <type_traits>

// remove referenct
template <typename T>
struct my_remove_reference {
    typedef T type;
};
template <typename T>
struct my_remove_reference<T&> {
    typedef T type;
};
template <typename T>
struct my_remove_reference<T&&> {
    typedef T type;
};

void test01 () {
    std::cout << "test01" << std::endl;
    std::cout << std::is_same<int, my_remove_reference<int>::type>::value << std::endl;
    std::cout << std::is_same<int, my_remove_reference<int&>::type>::value << std::endl;
    std::cout << std::is_same<int, my_remove_reference<int&&>::type>::value << std::endl;
}

// my move
template <typename T>
constexpr typename my_remove_reference<T>::type&& my_move(T&& t) noexcept {
    return static_cast<typename my_remove_reference<T>::type>(t);
}

void test02() {
    std::cout << "test02" << std::endl;

    int a = 1;
    std::cout << std::is_same<int&&, decltype(my_move(a))>::value << std::endl;
    std::cout << std::is_same<int&&, decltype(my_move<int&>(a))>::value << std::endl;

    int& b = a;
    std::cout << std::is_same<int&&, decltype(my_move(b))>::value << std::endl;
    std::cout << std::is_same<int&&, decltype(my_move<int&>(b))>::value << std::endl;

    std::cout << std::is_same<int&&, decltype(my_move(int(1)))>::value << std::endl;
    std::cout << std::is_same<int&&, decltype(my_move<int>(int(1)))>::value << std::endl;
    std::cout << std::is_same<int&&, decltype(my_move<int&&>(int(1)))>::value << std::endl;
}

// my forward
template <typename T>
constexpr T&& my_forward(typename my_remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& my_forward(typename my_remove_reference<T>::type&& t) noexcept {
    return static_cast<T&&>(t);
}

void test03() {
    std::cout << "test03" << std::endl;

    int a = 1;
    std::cout << std::is_same<int&&, decltype(my_forward<int>(a))>::value << std::endl;
    std::cout << std::is_same<int&, decltype(my_forward<int&>(a))>::value << std::endl;

    int& b = a;
    std::cout << std::is_same<int&&, decltype(my_forward<int>(b))>::value << std::endl;
    std::cout << std::is_same<int&, decltype(my_forward<int&>(a))>::value << std::endl;

    std::cout << std::is_same<int&&, decltype(my_forward<int>(int(1)))>::value << std::endl;
    std::cout << std::is_same<int&, decltype(my_forward<int&>(int(1)))>::value << std::endl;
    std::cout << std::is_same<int&&, decltype(my_forward<int&&>(int(1)))>::value << std::endl;
}

int main(int argc, char** argv) {
    test01();
    test02();
    test03();
    return 1;
}