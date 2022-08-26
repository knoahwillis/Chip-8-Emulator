#include "Display.hpp"


Display::Display() {
  window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Display::~Display() {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(window);
}

void Display::clear() {
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      screen[j][i] = false;
    }
  }
}