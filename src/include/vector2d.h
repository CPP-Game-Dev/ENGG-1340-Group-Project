#pragma once

/*
 * Struct to store positions(mainly) with plenty of functionality
 */

struct Vector2D {
    int y;
    int x;
    Vector2D();
    Vector2D(int y, int x);
    Vector2D(const Vector2D &vector2d);

    #pragma region Operator Overloads
    Vector2D &operator=(const Vector2D &rhs);       // Assignment
    bool operator==(const Vector2D &rhs) const;     // Comparison
    bool operator!=(const Vector2D &rhs) const;     // Inverse comparison
    Vector2D operator+(const Vector2D &rhs) const;  // Addition
    Vector2D &operator+=(const Vector2D &rhs);      // Addition + Assignment
    Vector2D operator-(const Vector2D &rhs) const;  // Subtraction
    Vector2D &operator-=(const Vector2D &rhs);      // Subtraction + Assignment
    #pragma endregion

    static inline Vector2D UnitX() { return Vector2D(0, 1); }
    static inline Vector2D UnitY() { return Vector2D(1, 0); }
};