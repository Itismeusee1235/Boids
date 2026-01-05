#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "./vec_math.hpp"
#include <SDL2/SDL.h>

struct Ray {
  Vector origin;
  Vector dir;
  double length;
};
struct RayHit {
  bool hit = false;
  double dist = 0.0;
  Vector point;
  Vector normal;
  void Draw(SDL_Renderer* renderer);
};

class Object {
  public:
  virtual bool rayCastCheck(Ray const& ray, RayHit& hit) const = 0;
  virtual bool containsCheck(Vector point) const = 0;
  virtual void Draw(SDL_Renderer* render) = 0;
};

class Circle : public Object {

  public:
  Vector centre;
  double radius;
  Circle()
      : centre(ZeroVec)
      , radius(1)
  {
  }
  Circle(Vector& c, double r);
  bool rayCastCheck(Ray const& ray, RayHit& hit) const override;
  bool containsCheck(Vector point) const override;
  void Draw(SDL_Renderer* render) override;
};

class Rectangle : public Object {
  public:
  Vector centre;
  double length;
  double height;
  Rectangle()
      : centre(ZeroVec)
      , length(0.0)
      , height(0.0)
  {
  }
  Rectangle(Vector& centre, double length, double height);

  bool rayCastCheck(Ray const& ray, RayHit& hit) const override;
  bool containsCheck(Vector point) const override;
  void Draw(SDL_Renderer* render) override;
};

#endif
