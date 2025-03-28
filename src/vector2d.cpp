#include "include/vector2d.h"

Vector2D::Vector2D() {
    this->y = 0;
    this->x = 0;
}

Vector2D::Vector2D(int y, int x) {
    this->y = y;
    this->x = x;
}

Vector2D::Vector2D(const Vector2D &vector2d) {
    this->y = vector2d.y;
    this->x = vector2d.x;
}

Vector2D &Vector2D::operator=(const Vector2D &rhs) {
    this->y = rhs.y;
    this->x = rhs.x;
    return *this;
}

bool Vector2D::operator==(const Vector2D &rhs) const {
    return (this->y == rhs.y && this->x == rhs.x);
}

bool Vector2D::operator!=(const Vector2D &rhs) const { return !(*this == rhs); }

Vector2D Vector2D::operator+(const Vector2D &rhs) const {
    return Vector2D(*this) += rhs;
}

Vector2D &Vector2D::operator+=(const Vector2D &rhs) {
    this->y += rhs.y;
    this->x += rhs.x;
    return *this;
}

Vector2D Vector2D::operator-(const Vector2D &rhs) const {
    return Vector2D(*this) -= rhs;
}

Vector2D &Vector2D::operator-=(const Vector2D &rhs) {
    this->y -= rhs.y;
    this->x -= rhs.x;
    return *this;
}
