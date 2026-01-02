#include "../include/boid.hpp"
#include <iostream>

void Boid::update(double deltaTime)
{
  pos += vel * deltaTime;
}

void Boid::print()
{
  std::cout << "pos : " << std::endl;
  pos.print();
  std::cout << "vel : " << std::endl;
  vel.print();
}
