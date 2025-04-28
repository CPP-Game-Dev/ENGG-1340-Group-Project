#include "../src/include/vector2d.h"
#include <cassert>
#include <iostream>

namespace vector2d_tests {

void testDefaultConstructor() {
    Vector2D v;
    assert(v.y == 0);
    assert(v.x == 0);
    std::cout << "Default constructor test passed!" << std::endl;
}

void testParameterizedConstructor() {
    Vector2D v(3, 4);
    assert(v.y == 3);
    assert(v.x == 4);
    std::cout << "Parameterized constructor test passed!" << std::endl;
}

void testCopyConstructor() {
    Vector2D v1(5, 7);
    Vector2D v2(v1);
    assert(v2.y == 5);
    assert(v2.x == 7);
    std::cout << "Copy constructor test passed!" << std::endl;
}

void testAssignmentOperator() {
    Vector2D v1(1, 2);
    Vector2D v2;
    v2 = v1;
    assert(v2.y == 1);
    assert(v2.x == 2);
    std::cout << "Assignment operator test passed!" << std::endl;
}

void testEqualityOperators() {
    Vector2D v1(1, 2);
    Vector2D v2(1, 2);
    Vector2D v3(3, 4);
    
    assert(v1 == v2);
    assert(!(v1 == v3));
    assert(v1 != v3);
    assert(!(v1 != v2));
    std::cout << "Equality operators test passed!" << std::endl;
}

void testAdditionOperators() {
    Vector2D v1(1, 2);
    Vector2D v2(3, 4);
    
    // Test operator+
    Vector2D v3 = v1 + v2;
    assert(v3.y == 4);
    assert(v3.x == 6);
    
    // Test operator+=
    v1 += v2;
    assert(v1.y == 4);
    assert(v1.x == 6);
    std::cout << "Addition operators test passed!" << std::endl;
}

void testSubtractionOperators() {
    Vector2D v1(5, 8);
    Vector2D v2(3, 4);
    
    // Test operator-
    Vector2D v3 = v1 - v2;
    assert(v3.y == 2);
    assert(v3.x == 4);
    
    // Test operator-=
    v1 -= v2;
    assert(v1.y == 2);
    assert(v1.x == 4);
    std::cout << "Subtraction operators test passed!" << std::endl;
}

void testStaticMethods() {
    Vector2D unitX = Vector2D::UnitX();
    assert(unitX.y == 0);
    assert(unitX.x == 1);
    
    Vector2D unitY = Vector2D::UnitY();
    assert(unitY.y == 1);
    assert(unitY.x == 0);
    std::cout << "Static methods test passed!" << std::endl;
}

void runAll() {
    std::cout << "Running Vector2D tests..." << std::endl;
    testDefaultConstructor();
    testParameterizedConstructor();
    testCopyConstructor();
    testAssignmentOperator();
    testEqualityOperators();
    testAdditionOperators();
    testSubtractionOperators();
    testStaticMethods();
    std::cout << "All Vector2D tests passed!" << std::endl;
}

} // namespace vector2d_tests
