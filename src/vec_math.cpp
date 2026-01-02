#include "../include/vec_math.hpp"
#include <iostream>

Vector Vector::operator+(const Vector& other) const
{
  return Vector(this->x + other.x, this->y + other.y);
}

Vector Vector::operator-(const Vector& other) const
{
  return Vector(this->x - other.x, this->y - other.y);
}

double Vector::operator*(const Vector& other) const
{
  return this->x * other.x + this->y * other.y;
}

Vector Vector::operator*(double other) const
{
  return Vector(this->x * other, this->y * other);
}

Vector& Vector::operator+=(const Vector& other)
{
  x += other.x;
  y += other.y;
  return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
  x -= other.x;
  y -= other.y;
  return *this;
}

Vector Vector::rotate(double rads) const
{
  double costheta = std::cos(rads);
  double sintheta = std::sin(rads);

  double dx = x * costheta - y * sintheta;
  double dy = y * costheta + x * sintheta;

  return Vector(dx, dy);
}

void Vector::normalize()
{
  double mag = this->norm();
  if (mag < 1e-12)
    return;

  this->x /= mag;
  this->y /= mag;
}

Vector Vector::normalized() const
{
  Vector out(this->x, this->y);
  out.normalize();
  return out;
}

void Vector::print()
{
  std::cout << "x: " << this->x << " ";
  std::cout << "y: " << this->y << " ";
  std::cout << "norm: " << this->norm() << std::endl;
}
