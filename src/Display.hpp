#pragma once
#include <SDL2/SDL.h>

enum Keyboard {
  ZERO = SDL_SCANCODE_X,
  ONE = SDL_SCANCODE_1,
  TWO = SDL_SCANCODE_2,
  THREE = SDL_SCANCODE_3,
  FOUR = SDL_SCANCODE_Q,
  FIVE = SDL_SCANCODE_W,
  SIX = SDL_SCANCODE_E,
  SEVEN = SDL_SCANCODE_A,
  EIGHT = SDL_SCANCODE_S,
  NINE = SDL_SCANCODE_D,
  A = SDL_SCANCODE_Z,
  B = SDL_SCANCODE_C,
  C = SDL_SCANCODE_4,
  D = SDL_SCANCODE_R,
  E = SDL_SCANCODE_F,
  F = SDL_SCANCODE_V
};

class Display {
protected:
  SDL_Renderer *rend;
  SDL_Window *window;

  const int width = 64;
  const int height = 32;

  bool screen[32][64];

public:
  Display();
  ~Display();

public:
  void renderFrame();
  void clear();
};