#include "../include/boid.hpp"
#include "../include/Objects.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <random>

std::random_device boid_rd;
std::mt19937 boids_gen(boid_rd());
std::uniform_real_distribution<double> WanderAngleGen(-1, 1);

void Boid::update(double deltaTime, Boid** Boid_List, int n, Object** Objects, int n_obj, SDL_Renderer* renderer)
{
  Vector steer(0, 0);

  Vector alignmentCentre(0, 0);
  Vector aggregationCentre(0, 0);
  int alignCount = 0;
  int aggCount = 0;

  Vector TotalSegregation(0, 0);
  Vector TotalAggregation(0, 0);
  Vector TotalAlignment(0, 0);

  double fov_angle = cos(FOV * 0.5);

  for (int i = 0; i < n; i++) {

    if (Boid_List[i] == this)
      continue;

    Vector dist = Boid_List[i]->pos - pos;

    if (dist.x > MAX_X / 2) {
      dist.x -= MAX_X;
    } else if (dist.x < -MAX_X / 2) {
      dist.x += MAX_X;
    }
    if (dist.y > MAX_Y / 2) {
      dist.y -= MAX_Y;
    } else if (dist.y < -MAX_Y / 2) {
      dist.y += MAX_Y;
    }

    double mag = dist.norm();
    Vector dir = dist.normalized();

    if (mag < SEGREGATION_RADIUS && mag > 0) {
      double strength = (SEGREGATION_RADIUS - mag) / SEGREGATION_RADIUS;
      if (strength < 0.0) {
        strength = 0.0;
      } else if (strength > 1.0) {
        strength = 1.0;
      }

      TotalSegregation += dir * (MAX_SEGREGATION_FORCE * strength);
    }

    if (vel.normalized() * dir < fov_angle) {
      continue;
    }

    if (mag < AGGREGATION_RADIUS && mag > 0) {
      aggregationCentre += dist;
      aggCount++;
    }
    if (mag < ALIGNMENT_RADIUS && mag > 0) {
      alignmentCentre += Boid_List[i]->vel;
      alignCount++;
    }
  }

  Vector centre = vel;
  centre.setMag(WANDER_DIST);
  centre += pos;

  Vector rotation = vel;
  rotation.setMag(WANDER_RADIUS);
  rotation = rotation.rotate(steeringAngle);
  centre += rotation;
  steer += (centre - pos);

  steeringAngle += MAX_RANDOM_STEER * WanderAngleGen(boids_gen);

  if (aggCount > 0) {
    aggregationCentre = aggregationCentre * (1.0 / aggCount);
    TotalAggregation += (aggregationCentre).normalized() * MAX_AGGREGATION_FORCE;
  }
  if (alignCount > 0) {
    alignmentCentre = alignmentCentre * (1.0 / alignCount);
    TotalAlignment += (alignmentCentre - vel).normalized() * MAX_ALIGNMENT_FORCE;
  }

  steer += AGGREGATION_W * TotalAggregation;
  steer -= SEGREGATION_W * TotalSegregation;
  steer += ALIGNMENT_W * TotalAlignment;

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
  for (int i = 0; i < n_obj; i++) {
    if (Objects[i]->rayCastCheck(r1, testHit)) {
      if (testHit.dist < hit.dist) {
        hit = testHit;
      }
    }
    if (Objects[i]->rayCastCheck(r2, testHit)) {
      if (testHit.dist < hit.dist) {
        hit = testHit;
      }
    }
    if (Objects[i]->rayCastCheck(r3, testHit)) {
      if (testHit.dist < hit.dist) {
        hit = testHit;
      }
    }
  }

  if (hit.dist <= AVOIDANCE_RADIUS) {
    steer += hit.normal * (MAX_AVOIDANCE_FORCE * 2) * ((AVOIDANCE_RADIUS - hit.dist) / AVOIDANCE_RADIUS);
    // hit.Draw(renderer);
  }

  vel += steer * deltaTime;

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
