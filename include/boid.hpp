#ifndef BOID_HPP
#define BOID_HPP

#include "./vec_math.hpp"
#include <cstdlib>
#include <ctime>

class Boid {

  public:
  Vector pos;
  Vector vel;

  int MAX_X;
  int MAX_Y;
  const double MAX_VEL = 160;
  const double WANDER_DIST = 2;
  const double WANDER_RADIUS = 8;
  const double MAX_RANDOM_STEER = M_PI / 3.0; // In Radians
  const double MAX_WANDER_FORCE = 7000;
  const double MAX_AGGREGATION_FORCE = 4000;
  const double MAX_SEGREGATION_FORCE = 9000;
  const double MAX_ALIGNMENT_FORCE = 5000;
  const double AGGREGATION_RADIUS = 50;
  const double SEGREGATION_RADIUS = 15;
  double steeringAngle = 0.0; // In Radians

  Boid(int worldWidth, int worldHeight)
      : pos(ZeroVec)
      , vel(ZeroVec)
      , MAX_X(worldWidth)
      , MAX_Y(worldHeight)
  {
    srand(static_cast<unsigned int>(std::time(nullptr)));
  }

  Boid(Vector pos_, Vector vel_, int worldWidth, int worldHeight)
      : pos(pos_)
      , vel(vel_)
      , MAX_X(worldWidth)
      , MAX_Y(worldHeight)
  {
    srand(static_cast<unsigned int>(std::time(nullptr)));
  }

  double rotation() { return vel.rotation() * (180.0 / M_PI); }
  void update(double deltaTime, Boid** Boid_List, int n);
  void print();
};

#endif
