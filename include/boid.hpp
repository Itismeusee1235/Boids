#ifndef BOID_HPP
#define BOID_HPP

#include "./vec_math.hpp"
#include <SDL2/SDL.h>
#include "./Objects.hpp"
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

  const double WANDER_DIST = 10;
  const double WANDER_RADIUS = 20;
  const double MAX_RANDOM_STEER = M_PI / 3.0; // In Radians

  const double MAX_WANDER_FORCE = 100;
  const double MAX_COHESION_FORCE = 80;
  const double MAX_SEPARATION_FORCE = 200;
  const double MAX_ALIGNMENT_FORCE = 50;
  const double MAX_AVOIDANCE_FORCE = 400;

  const double COHENSION_RADIUS = 100;
  const double SEPARATION_RADIUS = 50;
  const double ALIGNMENT_RADIUS = 50;
  const double AVOIDANCE_RADIUS = 40;

  const double AGGREGATION_W = 1;
  const double SEGREGATION_W = 1;
  const double ALIGNMENT_W = 1;

  const double FOV = 2 * M_PI / 3;
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
  void update(double deltaTime, Boid** Boid_List, int n, Object** Objects, int obj_n, SDL_Renderer* renderer);

  Vector Cohesion(Boid** Boid_List, int n);
  Vector Separation(Boid** Boid_List, int n);
  Vector Alignement(Boid** Boid_List, int n);
  Vector ObjectAvoidance(Object** Object_List, int n);
  Vector Wandering();

  void Draw(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* textRect);
  void print();
};

#endif
