#ifndef VEC_MATH_HPP
#define VEC_MATH_HPP

#include <cmath>

struct Vector {
  double x;
  double y;

  double norm() const { return std::sqrt(x * x + y * y); };

  Vector()
      : x(0)
      , y(0)
  {
  }
  Vector(double x_, double y_)
      : x(x_)
      , y(y_)
  {
  }

  Vector operator+(const Vector& other) const;
  Vector operator-(const Vector& other) const;
  Vector operator*(double scalar) const; // scalar multiplication
  double operator*(const Vector& other) const; // dot product
  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);

  void normalize();
  Vector normalized() const;
  Vector rotate(double rads) const;
  double rotation() { return atan2(y, x); };
  void setMag(double mag);

  void print();
};

inline Vector operator*(double s, const Vector& v)
{
  return v * s;
}

const Vector ZeroVec(0, 0);

#endif
