#include "../include/boid.hpp"
#include "../include/Objects.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <algorithm>

std::random_device boid_rd;
std::mt19937 boids_gen(boid_rd());
std::uniform_real_distribution<double> WanderAngleGen(-1, 1);

Vector Boid::Cohesion(Boid** Boid_List, int n)
{
  // Returns the force to be applied to velocity
  Vector sum(0, 0);
  int count = 0;
  double fov_angle = cos(FOV * 0.5);

  for (int i = 0; i < n; i++) {
    Vector diff = Boid_List[i]->pos - pos;
    double dist = diff.norm();
    Vector dir = diff.normalized();

    if (dir * vel.normalized() < fov_angle || dist > COHENSION_RADIUS) {
      continue;
    }
    sum += Boid_List[i]->pos;
    count++;
  }

  if (count > 0) {
    sum = sum * (1.0 / count); // Average of positions
    Vector target = sum - pos;

    target.setMag(MAX_VEL);

    Vector diff = target - vel;
    diff.setMag(MAX_COHESION_FORCE);
    return diff;
  } else {
    return ZeroVec;
  }
}

Vector Boid::Separation(Boid** Boid_List, int n)
{
  Vector sum(0, 0);
  for (int i = 0; i < n; i++) {
    Vector diff = Boid_List[i]->pos - pos;
    double dist = diff.norm();
    Vector dir = diff.normalized();

    if (dist > SEPARATION_RADIUS) {
      continue;
    }

    double strength = (MAX_SEPARATION_FORCE - dist) / (MAX_SEPARATION_FORCE);
    std::clamp(strength, 0.0, 1.0);
    sum += dir * MAX_SEPARATION_FORCE * strength;
  }
  return sum;
}

Vector Boid::Alignement(Boid** Boid_List, int n)
{
  Vector sum(0, 0);
  int count = 0;
  double fov_angle = cos(FOV * 0.5);

  for (int i = 0; i < n; i++) {
    Vector diff = Boid_List[i]->pos - pos;
    double dist = diff.norm();
    Vector dir = diff.normalized();

    if (dir * vel.normalized() < fov_angle || dist > ALIGNMENT_RADIUS) {
      continue;
    }
    sum += Boid_List[i]->vel - vel;
    count++;
  }

  if (count > 0) {
    sum = sum * (1.0 / count);
    Vector diff = sum - vel;
    diff.setMag(MAX_ALIGNMENT_FORCE);
    return diff;
  } else {
    return ZeroVec;
  }
}

Vector Boid::ObjectAvoidance(Object** Object_List, int n)
{

  Vector sum(0, 0);

  Ray r1, r2, r3;
  r1.origin = pos + vel.normalized() * 8;
  r1.dir = vel.normalized();
  r1.length = 24;

  r2.origin = pos + vel.normalized() * 8;
  r2.dir = vel.normalized().rotate(M_PI / 6);
  r2.length = 26;

  r3.origin = pos + vel.normalized() * 8;
  r3.dir = vel.normalized().rotate(-M_PI / 6);
  r3.length = 24;

  RayHit hit;
  hit.dist = MAXFLOAT;
  RayHit testHit;
  for (int i = 0; i < n; i++) {
    if (Object_List[i]->rayCastCheck(r1, testHit)) {
      if (testHit.dist < hit.dist) {
        hit = testHit;
      }
    }
    if (Object_List[i]->rayCastCheck(r2, testHit)) {
      if (testHit.dist < hit.dist) {
        hit = testHit;
      }
    }
    if (Object_List[i]->rayCastCheck(r3, testHit)) {
      if (testHit.dist < hit.dist) {
        hit = testHit;
      }
    }
  }

  if (hit.dist <= AVOIDANCE_RADIUS) {
    sum += hit.normal * (MAX_AVOIDANCE_FORCE * 2) * ((AVOIDANCE_RADIUS - hit.dist) / AVOIDANCE_RADIUS);
    // hit.Draw(renderer);
  }
  return sum;
}

Vector Boid::Wandering()
{
  Vector circle_centre = vel;
  circle_centre.setMag(WANDER_DIST);
  circle_centre += pos;

  Vector rotation = vel;
  rotation.setMag(WANDER_RADIUS);
  rotation = rotation.rotate(steeringAngle);
  circle_centre += rotation;

  Vector target = (circle_centre - pos).normalized() * vel.norm();

  steeringAngle += MAX_RANDOM_STEER * WanderAngleGen(boids_gen);
  return target;
}

void Boid::update(double deltaTime, Boid** Boid_List, int n, Object** Objects, int n_obj, SDL_Renderer* renderer)
{
  Vector TotalSteer(0, 0);

  Vector CohesionSteer = Cohesion(Boid_List, n);
  Vector SeparationSteer = Separation(Boid_List, n);
  Vector AlignmentSteer = Alignement(Boid_List, n);
  Vector AvoidanceSteer = ObjectAvoidance(Objects, n_obj);

  TotalSteer += CohesionSteer;
  TotalSteer -= SeparationSteer;
  TotalSteer += AlignmentSteer;
  TotalSteer += Wandering();
  TotalSteer += AvoidanceSteer;

  vel += TotalSteer * deltaTime;

  if (vel.norm() > this->MAX_VEL) {
    vel.setMag(this->MAX_VEL);
  } else if (vel.norm() < this->MIN_VEL) {
    vel.setMag(this->MIN_VEL);
  }

  pos += vel * deltaTime;

  // if (pos.x > MAX_X) {
  //   pos.x -= MAX_X;
  // } else if (pos.x < 0) {
  //   pos.x += MAX_X;
  // }
  //
  // if (pos.y > MAX_Y) {
  //   pos.y -= MAX_Y;
  // } else if (pos.y < 0) {
  //   pos.y += MAX_Y;
  // }

  if (pos.x < margin && vel.x < 0) {
    vel.x += turnfactor;
  } else if (pos.x > (MAX_X - margin) && vel.x > 0) {
    vel.x -= turnfactor;
  }

  if (pos.y < margin && vel.y < 0) {
    vel.y += turnfactor;
  } else if (pos.y > (MAX_Y - margin) && vel.y > 0) {
    vel.y -= turnfactor;
  }
}

void Boid::print()
{
  std::cout << "pos : " << std::endl;
  pos.print();
  std::cout << "vel : " << std::endl;
  vel.print();
}

void Boid::Draw(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* texRect)
{
  // Vector src = pos + vel.normalized() * 8;
  // Vector end = src + vel.normalized() * 18;
  // Vector end1 = src + (vel.normalized().rotate(M_PI / 6) * 16);
  // Vector end2 = src + (vel.normalized().rotate(-M_PI / 6) * 16);
  // SDL_RenderDrawLine(renderer, src.x, src.y, end.x, end.y);
  // SDL_RenderDrawLine(renderer, src.x, src.y, end1.x, end1.y);
  // SDL_RenderDrawLine(renderer, src.x, src.y, end2.x, end2.y);
  SDL_RenderCopyEx(renderer, texture, NULL, texRect, this->rotation() + 90, NULL, SDL_FLIP_NONE);
}
