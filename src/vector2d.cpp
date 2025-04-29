#include "include/vector2d.h"

/*
* Default constructor
*
* Initializes x and y to 0
*
* @return none
*/
Vector2D::Vector2D() {
    this->y = 0;
    this->x = 0;
}

/*
* Parameterized constructor
*
* Initializes a vector with given y and x values
*
* @param y The vertical component
* @param x The horizontal component
* @return none
*/
Vector2D::Vector2D(int y, int x) {
    this->y = y;
    this->x = x;
}


/*
* Copy constructor
*
* Creates a new Vector2D by copying values from another vector
*
* @param vector2d The vector to copy from
* @return none
*/
Vector2D::Vector2D(const Vector2D &vector2d) {
    this->y = vector2d.y;
    this->x = vector2d.x;
}


/*
* Assignment operator
*
* Assigns the values of another vector to this vector
*
* @param rhs The vector to assign from
* @return Vector2D& Reference to this vector
*/
Vector2D &Vector2D::operator=(const Vector2D &rhs) {
    this->y = rhs.y;
    this->x = rhs.x;
    return *this;
}

/*
* Equality operator and Inequality operator
*
* Compares this vector with another for equality
* Compares if this vector is not equal to another
*
* @param rhs The vector to compare with
* @return bool True if both vectors have the same x and y
*/
bool Vector2D::operator==(const Vector2D &rhs) const {
    return (this->y == rhs.y && this->x == rhs.x);
}

bool Vector2D::operator!=(const Vector2D &rhs) const { return !(*this == rhs); }

/*
* Addition operator
*
* Adds this vector and another, returns a new vector
*
* @param rhs The vector to add
* @return Vector2D The resulting vector
*/
Vector2D Vector2D::operator+(const Vector2D &rhs) const {
    return Vector2D(*this) += rhs;
}
/*
* Addition assignment operator
*
* Adds another vector to this one and assigns the result
*
* @param rhs The vector to add
* @return Vector2D& Reference to this updated vector
*/

Vector2D &Vector2D::operator+=(const Vector2D &rhs) {
    this->y += rhs.y;
    this->x += rhs.x;
    return *this;
}

/*
* Subtraction operator
*
* Subtracts another vector from this one, returns a new vector
*
* @param rhs The vector to subtract
* @return Vector2D The resulting vector
*/

Vector2D Vector2D::operator-(const Vector2D &rhs) const {
    return Vector2D(*this) -= rhs;
}
/*
* Subtraction assignment operator
*
* Subtracts another vector from this one and assigns the result
*
* @param rhs The vector to subtract
* @return Vector2D& Reference to this updated vector
*/
Vector2D &Vector2D::operator-=(const Vector2D &rhs) {
    this->y -= rhs.y;
    this->x -= rhs.x;
    return *this;
}
