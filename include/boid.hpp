#ifndef BOID_HPP
#define BOID_HPP

#include "./vec_math.hpp"

class Boid {

  public:
  Vector pos;
  Vector vel;

  Boid()
      : pos(ZeroVec)
      , vel(ZeroVec)
  {
  }

  Boid(Vector pos_, Vector vel_)
      : pos(pos_)
      , vel(vel_)
  {
  }

  void update(double deltaTime);
  void print();
};

#endif
