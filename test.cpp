#include "./include/Objects.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> C_X(50, 750);
uniform_int_distribution<int> C_Y(50, 550);
uniform_int_distribution<int> C_R(10, 50);
uniform_int_distribution<int> Color(10, 255);

const int obj_num = 20;

void SDL_DrawCircle(SDL_Renderer* renderer, Vector& centre, double radius)
{
  for (int w = -radius; w <= radius; w++) {
    int y = sqrt(radius * radius - w * w);
    SDL_RenderDrawLine(renderer, (int)(w + centre.x), (int)(-y + centre.y), (int)(w + centre.x), (int)(y + centre.y));
  }
}

int main()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init SDL %s", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow("Boids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Error Making window : %s", SDL_GetError());
    return -1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    printf("Failed to maker renderer : %s", SDL_GetError());
    return -1;
  }

  Object* obstacles[obj_num];
  for (int i = 0; i < obj_num; i++) {
    int type = C_R(gen) % 2;
    Vector center;
    center.x = C_X(gen);
    center.y = C_Y(gen);
    if (type == 0) {
      obstacles[i] = new Rectangle(center, C_R(gen), C_R(gen));
    } else {
      obstacles[i] = new Circle(center, C_R(gen));
    }
  }

  int M_X;
  int M_Y;

  Vector M_pos;
  Vector origin(400, 300);

  Ray r;
  RayHit hit;

  bool exit = false;
  while (!exit) {

    SDL_GetMouseState(&M_X, &M_Y);
    M_pos.x = M_X;
    M_pos.y = M_Y;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        exit = true;
        break;
      }
      if (ev.type == SDL_MOUSEBUTTONDOWN) {
        origin = M_pos;
      }
    }

    r.origin = origin;
    r.dir = M_pos - origin;
    r.length = r.dir.norm();
    r.dir.normalize();

    bool didHit = false;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, (int)origin.x, (int)origin.y, M_X, M_Y);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int i = 0; i < obj_num; i++) {
      obstacles[i]->Draw(renderer);
    }

    double minDist = MAXFLOAT;
    RayHit testHit;
    for (int i = 0; i < obj_num; i++) {
      if (obstacles[i]->rayCastCheck(r, testHit)) {
        if (testHit.dist < minDist) {
          minDist = testHit.dist;
          hit = testHit;
          didHit = true;
        }
      }
    }

    if (didHit) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
      SDL_DrawCircle(renderer, hit.point, 10);

      SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
      Vector end = hit.point + hit.normal * 20;
      SDL_RenderDrawLine(renderer, (int)hit.point.x, (int)hit.point.y, (int)end.x, (int)end.y);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_Quit();
  return 0;
}
