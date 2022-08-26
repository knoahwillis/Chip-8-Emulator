#pragma once
#include <SDL2/SDL.h>

class Display {
protected:
  SDL_Renderer *rend;
  SDL_Window *window;

  int width = 64;
  int height = 32;

  bool screen[32][64];

public:
  Display();
  ~Display();

public:
  // void render();
  void clear();
};