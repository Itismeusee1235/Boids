#include "../include/Objects.hpp"
#include <algorithm>
#include <SDL2/SDL.h>

void RayHit::Draw(SDL_Renderer* renderer)
{
  int radius = 5;
  Vector centre = point;

  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

  for (int w = -radius; w <= radius; w++) {
    int y = sqrt(radius * radius - w * w);
    SDL_RenderDrawLine(renderer, (int)(w + centre.x), (int)(-y + centre.y), (int)(w + centre.x), (int)(y + centre.y));
  }
}

Circle::Circle(Vector& c, double r)
{
  this->centre = c;
  this->radius = r;
}

bool Circle::rayCastCheck(Ray const& ray, RayHit& hit) const
{
  // This returns only Entry Hit
  // This doesnt return Exit Hit
  Vector A = ray.origin;
  Vector AC = centre - A;

  double projection = (AC * ray.dir);
  Vector P = projection * ray.dir + A;

  double dist2 = (P - centre) * (P - centre);
  if (dist2 > radius * radius) {
    return false;
  }

  double HPMag = sqrt(radius * radius - dist2);
  double t_entry = projection - HPMag;
  double t_exit = projection + HPMag;

  double t0 = std::max(t_entry, 0.0);
  double t1 = std::min(t_exit, ray.length);

  if (t0 > t1) { // If Entry dist > Exit dist .i.e Entry happened before exit
    return false;
  }

  double dist = (t_entry >= 0.0) ? t_entry : t_exit;

  Vector H = P - HPMag * ray.dir;

  Vector hitNormal = (H - centre).normalized();

  hit.dist = projection - HPMag;
  hit.hit = true;
  hit.point = H;
  hit.normal = hitNormal;
  return true;
}

void Circle::Draw(SDL_Renderer* renderer)
{

  for (int w = -radius; w <= radius; w++) {
    int y = sqrt(radius * radius - w * w);
    SDL_RenderDrawLine(renderer, (int)(w + centre.x), (int)(-y + centre.y), (int)(w + centre.x), (int)(y + centre.y));
  }
}

bool Circle::containsCheck(Vector point) const
{
  Vector diff = point - centre;
  if (diff.norm() > radius) {
    return false;
  }
  return true;
}

Rectangle::Rectangle(Vector& c, double l, double h)
{
  this->centre = c;
  this->length = l;
  this->height = h;
}

bool Rectangle::rayCastCheck(Ray const& ray, RayHit& hit) const
{
  Vector bottomLeft;
  Vector topRight;

  bottomLeft.x = centre.x - (length / 2);
  bottomLeft.y = centre.y - (height / 2);

  topRight.x = centre.x + (length / 2);
  topRight.y = centre.y + (height / 2);

  // X - Slab - defined by x
  if (ray.dir.x == 0 && (ray.origin.x < bottomLeft.x || ray.origin.x > topRight.x)) {
    return false;
  }

  double t0 = (bottomLeft.x - ray.origin.x) / ray.dir.x;
  double t1 = (topRight.x - ray.origin.x) / ray.dir.x;
  double t_near_x = std::min(t0, t1);
  double t_far_x = std::max(t0, t1);

  // Y - Slab defined by y
  if (ray.dir.y == 0 && (ray.origin.y < bottomLeft.y || ray.origin.y > topRight.y)) {
    return false;
  }

  t0 = (bottomLeft.y - ray.origin.y) / ray.dir.y;
  t1 = (topRight.y - ray.origin.y) / ray.dir.y;
  double t_near_y = std::min(t0, t1);
  double t_far_y = std::max(t0, t1);

  double t_near = std::max(t_near_x, t_near_y);
  double t_far = std::min(t_far_x, t_far_y);

  if (t_near > t_far || t_far < 0 || t_near > ray.length) {
    return false;
  }

  hit.point = ray.origin + ray.dir * t_near;
  hit.dist = t_near;

  Vector normal;
  if (t_near == t_near_x) {
    normal = (ray.dir.x < 0) ? Vector(1, 0) : Vector(-1, 0);
  } else {
    normal = (ray.dir.y < 0) ? Vector(0, 1) : Vector(0, -1);
  }

  hit.normal = normal;
  return true;
}

void Rectangle::Draw(SDL_Renderer* renderer)
{
  SDL_Rect rec;
  rec.w = (int)length;
  rec.h = (int)height;
  rec.x = (int)(centre.x - rec.w / 2);
  rec.y = (int)(centre.y - rec.h / 2);
  SDL_RenderFillRect(renderer, &rec);
}

bool Rectangle::containsCheck(Vector point) const
{
  Vector diff = point - centre;
  if (std::fabs(diff.x) > length / 2 || std::fabs(diff.y) > height / 2) {
    return false;
  }
  return true;
}
