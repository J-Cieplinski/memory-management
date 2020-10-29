#include <iostream>
#include <string>
#include "unique_ptr.hpp"

int main() {
    cs::unique_ptr<int> ptr(new int());
    *ptr = 10;
    std::cout << "int: " << *ptr << "\n";
    auto a = ptr.get();
    std::cout << "equal: " << (a == ptr.get()) << "\n";
    ptr.reset(new int(20));
    std::cout << "int before move after reset: " << *ptr << "\n";

    std::cout << "ptr before move: " << ptr.get() << "\n";
    cs::unique_ptr<int> ptr2(std::move(ptr));
    std::cout << "ptr: " << ptr.get() << "\n";
    std::cout << "ptr2: " << ptr2.get() << "\n";

    auto b = ptr2.release();
    std::cout << "ptr: " << ptr2.get() << "\n";
    std::cout << "b: " << b << "\n";

    cs::unique_ptr<std::string> str(new std::string{"asdasdasda"});
    std::cout << "string: " << str->at(1) << "\n";
    cs::unique_ptr<std::string> str1(new std::string{"aftujkfhgfd"});
    std::cout << "string: " << str1->at(1) << "\n";
    std::cout << "string: " << *str1 << "\n";
}
