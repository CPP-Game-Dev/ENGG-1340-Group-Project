#include "../src/include/utils.h"
#include <cassert>
#include <iostream>
#include <string>

namespace utils_tests {

// Custom class to test make_unique with constructors that take arguments
class TestClass {
  public:
    int a;
    std::string b;

    TestClass(int a_val, std::string b_val) : a(a_val), b(b_val) {}

    bool operator==(const TestClass &other) const {
        return a == other.a && b == other.b;
    }
};

void test_make_unique_int() {
    // Create a unique_ptr to an integer
    auto int_ptr = utils::make_unique<int>(42);
    assert(int_ptr != nullptr);
    assert(*int_ptr == 42);
    std::cout << "make_unique with int test passed!" << std::endl;
}

void test_make_unique_string() {
    // Create a unique_ptr to a string
    auto string_ptr = utils::make_unique<std::string>("hello world");
    assert(string_ptr != nullptr);
    assert(*string_ptr == "hello world");
    std::cout << "make_unique with string test passed!" << std::endl;
}

void test_make_unique_custom_class() {
    // Create a unique_ptr to a custom class with constructor arguments
    auto class_ptr = utils::make_unique<TestClass>(123, "test string");
    assert(class_ptr != nullptr);
    assert(class_ptr->a == 123);
    assert(class_ptr->b == "test string");
    std::cout << "make_unique with custom class test passed!" << std::endl;
}

void test_unique_ptr_ownership() {
    // Verify that unique_ptr properly manages ownership
    auto ptr = utils::make_unique<int>(100);
    auto raw_ptr = ptr.get(); // Get the raw pointer

    // Transfer ownership
    auto ptr2 = std::move(ptr);
    assert(ptr == nullptr);        // Original pointer should be null after move
    assert(ptr2.get() == raw_ptr); // New pointer should point to same object
    std::cout << "unique_ptr ownership test passed!" << std::endl;
}

void runAll() {
    std::cout << "Running utils tests..." << std::endl;
    test_make_unique_int();
    test_make_unique_string();
    test_make_unique_custom_class();
    test_unique_ptr_ownership();
    std::cout << "All utils tests passed!" << std::endl;
}

} // namespace utils_tests