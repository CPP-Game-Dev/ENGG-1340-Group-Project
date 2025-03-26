#pragma once

/*
 * Struct to store positions(mainly) with plenty of functionality
 */

struct Vector2D {
    int y;
    int x;
    Vector2D();
    Vector2D(int y, int x);

    Vector2D &operator=(const Vector2D &rhs);   // Assignment
    bool operator==(const Vector2D &rhs) const; // Comparison
    bool operator!=(const Vector2D &rhs) const; // Inverse comparison
    Vector2D &operator+(const Vector2D &rhs);   // Addition
    Vector2D &operator+=(const Vector2D &rhs);  // Addition + Assignment
    Vector2D &operator-(const Vector2D &rhs);   // Subtraction
    Vector2D &operator-=(const Vector2D &rhs);  // Subtraction + Assignment
};
