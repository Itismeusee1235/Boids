#include "../include/boid.hpp"
#include <iostream>
#include <random>

std::random_device boid_rd;
std::mt19937 boids_gen(boid_rd());
std::uniform_real_distribution<double> WanderAngleGen(-1, 1);

void Boid::update(double deltaTime, Boid** Boid_List, int n)
{
  Vector aggregationCentre(0, 0);
  Vector segregationCentre(0, 0);
  Vector alignmentCentre(0, 0);
  int aggCount = 0;
  int segCount = 0;
  int alignCount = 0;

  for (int i = 0; i < n; i++) {

    if (Boid_List[i] == this)
      continue;

    Vector dist = Boid_List[i]->pos - pos;
    double mag = dist.norm();

    if (mag < AGGREGATION_RADIUS) {
      aggregationCentre += Boid_List[i]->pos;
      aggCount++;
      alignmentCentre += Boid_List[i]->vel;
      alignCount++;
    }
    if (mag < SEGREGATION_RADIUS) {
      segregationCentre += Boid_List[i]->pos;
      segCount++;
    }
  }

  Vector steer(0, 0);

  Vector centre = vel;
  centre.setMag(WANDER_DIST);
  centre += pos;

  Vector rotation = vel;
  rotation.setMag(WANDER_RADIUS);
  rotation = rotation.rotate(steeringAngle);
  centre += rotation;
  steer += (centre - pos);

  steeringAngle += MAX_RANDOM_STEER * WanderAngleGen(boids_gen);
  // steer = steer.normalized() * MAX_WANDER_FORCE;

  if (aggCount > 0) {
    aggregationCentre = aggregationCentre * (1.0 / aggCount);
    steer += (aggregationCentre - pos).normalized() * MAX_AGGREGATION_FORCE;
  }
  if (segCount > 0) {
    segregationCentre = segregationCentre * (1.0 / segCount);
    steer -= (segregationCentre - pos).normalized() * MAX_SEGREGATION_FORCE;
  }
  if (alignCount > 0) {
    alignmentCentre = alignmentCentre * (1.0 / alignCount);
    steer += (alignmentCentre - vel).normalized() * MAX_ALIGNMENT_FORCE;
  }
  vel += steer * deltaTime;

  if (vel.norm() > this->MAX_VEL) {
    vel.setMag(this->MAX_VEL);
  }

  pos += vel * deltaTime;

  if (pos.x > MAX_X) {
    pos.x -= MAX_X;
  } else if (pos.x < 0) {
    pos.x += MAX_X;
  }

  if (pos.y > MAX_Y) {
    pos.y -= MAX_Y;
  } else if (pos.y < 0) {
    pos.y += MAX_Y;
  }
}

void Boid::print()
{
  std::cout << "pos : " << std::endl;
  pos.print();
  std::cout << "vel : " << std::endl;
  vel.print();
}
