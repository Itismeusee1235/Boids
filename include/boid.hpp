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
  const double MAX_VEL = 80;
  const double_t MIN_VEL = 10;

  const int margin = 10;
  const double turnfactor = 8;

  const double WANDER_DIST = 2;
  const double WANDER_RADIUS = 10;
  const double MAX_RANDOM_STEER = M_PI / 3.0; // In Radians

  const double MAX_WANDER_FORCE = 100;
  const double MAX_AGGREGATION_FORCE = 80;
  const double MAX_SEGREGATION_FORCE = 200;
  const double MAX_ALIGNMENT_FORCE = 70;

  const double AGGREGATION_RADIUS = 100;
  const double SEGREGATION_RADIUS = 50;
  const double ALIGNMENT_RADIUS = 50;

  const double AGGREGATION_W = 1;
  const double SEGREGATION_W = 1;
  const double ALIGNMENT_W = 1;

  const double FOV = 4 * M_PI / 3;
  double steeringAngle = 0.0; // In Radians

  Boid(int worldWidth, int worldHeight)
      : pos(ZeroVec)
      , vel(ZeroVec)
      , MAX_X(worldWidth)
      , MAX_Y(worldHeight)
  {
  }

  Boid(Vector pos_, Vector vel_, int worldWidth, int worldHeight)
      : pos(pos_)
      , vel(vel_)
      , MAX_X(worldWidth)
      , MAX_Y(worldHeight)
  {
  }

  double rotation() { return vel.rotation() * (180.0 / M_PI); }
  void update(double deltaTime, Boid** Boid_List, int n, Vector* Object_List, int obj_n);
  void print();
};

#endif
