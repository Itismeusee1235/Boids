#include "../include/vec_math.hpp"
#include "../include/boid.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <random>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int scale = 1;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> PosXGen(0, WIDTH* scale);
uniform_real_distribution<double> PosYGen(0, HEIGHT* scale);
uniform_real_distribution<double> AngleGen(0, 2 * M_PI);
uniform_real_distribution<double> MagGen(0, 160);

const string BOID_IMG_PATH = "/home/fenrir/Programming/C++/Boids/Bird.png";
const int num = 100;

int main()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init SDL %s", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow("Boids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Error Making window : %s", SDL_GetError());
    return -1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    printf("Failed to maker renderer : %s", SDL_GetError());
    return -1;
  }

  int IMG_FLAGS = IMG_INIT_PNG;
  if (!(IMG_Init(IMG_FLAGS) & IMG_FLAGS)) {
    printf("Failed to initialized for png loading : %s ", SDL_GetError());
    return -1;
  }

  SDL_Texture* boidTexture;

  SDL_Surface* loadedSurface = IMG_Load(BOID_IMG_PATH.c_str());
  if (loadedSurface == NULL) {
    printf("Unable to load image : %s", IMG_GetError());
    return -1;
  }
  boidTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
  if (boidTexture == NULL) {
    printf("Failed to create texture from surface : %s", SDL_GetError());
    return -1;
  }
  SDL_FreeSurface(loadedSurface);

  SDL_Rect textureRect;
  textureRect.w = 16 / scale;
  textureRect.h = 16 / scale;

  Boid* Boid_List[num];
  Vector* Object_List[1];
  Object_List[0] = new Vector(0, 0);

  for (int i = 0; i < num; i++) {
    Vector pos(PosXGen(gen), PosYGen(gen));
    Vector vel(1, 0);
    vel = vel.rotate(AngleGen(gen));
    vel.setMag(MagGen(gen));

    vel.print();
    Boid_List[i] = new Boid(pos, vel, WIDTH * scale, HEIGHT * scale);
  }

  Boid_List[0]->pos = Vector(400, 300);
  Boid_List[0]->pos = Vector(380, 300);

  Uint64 last = SDL_GetPerformanceCounter();
  Uint64 now;
  double deltaTime;

  bool exit = false;
  bool step = false;
  while (!exit) {
    now = SDL_GetPerformanceCounter();
    deltaTime = (double)(now - last) / (double)SDL_GetPerformanceFrequency();
    last = now;
    // cout << 1 / deltaTime << " " << SDL_GetPerformanceFrequency() << endl;

    int m_X;
    int m_Y;
    SDL_GetMouseState(&m_X, &m_Y);
    Vector mouse(m_X, m_Y);

    *Object_List[0] = mouse;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        exit = true;
        break;
      } else if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.sym == SDLK_q) {
          exit = true;
          break;
        }
        if (ev.key.keysym.sym == SDLK_SPACE) {
          step = !step;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int i = 0; i < num; i++) {
      textureRect.x = static_cast<int>(Boid_List[i]->pos.x) - 8;
      textureRect.y = static_cast<int>(Boid_List[i]->pos.y) - 8;
      textureRect.x /= scale;
      textureRect.y /= scale;
      SDL_RenderCopyEx(renderer, boidTexture, NULL, &textureRect, Boid_List[i]->rotation() + 90, NULL, SDL_FLIP_NONE);

      if (step) {
        Boid_List[i]->update(deltaTime, Boid_List, num, *Object_List, 1);
      }
    }

    // SDL_RenderCopy(renderer, boidTexture, NULL, &rex);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(boidTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  SDL_Quit();

  return 0;
}
