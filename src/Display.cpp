#include "Display.hpp"

#include <thread>

Display::Display() {
  window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * 10, height * 10, SDL_WINDOW_SHOWN);
  rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      screen[i][j] = 0;
    }
  }
}

Display::~Display() {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(window);
}

void Display::clear() {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      screen[i][j] = 0;
    }
  }
}

void Display::beginFrame() {
  startTime = std::chrono::system_clock::now();
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.scancode) {
      case SDL_SCANCODE_ESCAPE:
        running = false;
        break;
      case static_cast<int>(Keyboard::ZERO):
        keysPressed[0] = true;
        break;
      case static_cast<int>(Keyboard::ONE):
        keysPressed[1] = true;
        break;
      case static_cast<int>(Keyboard::TWO):
        keysPressed[2] = true;
        break;
      case static_cast<int>(Keyboard::THREE):
        keysPressed[3] = true;
        break;
      case static_cast<int>(Keyboard::FOUR):
        keysPressed[4] = true;
        break;
      case static_cast<int>(Keyboard::FIVE):
        keysPressed[5] = true;
        break;
      case static_cast<int>(Keyboard::SIX):
        keysPressed[6] = true;
        break;
      case static_cast<int>(Keyboard::SEVEN):
        keysPressed[7] = true;
        break;
      case static_cast<int>(Keyboard::EIGHT):
        keysPressed[8] = true;
        break;
      case static_cast<int>(Keyboard::NINE):
        keysPressed[9] = true;
        break;
      case static_cast<int>(Keyboard::A):
        keysPressed[0xA] = true;
        break;
      case static_cast<int>(Keyboard::B):
        keysPressed[0xB] = true;
        break;
      case static_cast<int>(Keyboard::C):
        keysPressed[0xC] = true;
        break;
      case static_cast<int>(Keyboard::D):
        keysPressed[0xD] = true;
        break;
      case static_cast<int>(Keyboard::E):
        keysPressed[0xE] = true;
        break;
      case static_cast<int>(Keyboard::F):
        keysPressed[0xF] = true;
        break;
      default:
        break;
      }
      break;
    case SDL_KEYUP:
      switch (e.key.keysym.scancode) {
      case static_cast<int>(Keyboard::ZERO):
        keysPressed[0] = false;
        break;
      case static_cast<int>(Keyboard::ONE):
        keysPressed[1] = false;
        break;
      case static_cast<int>(Keyboard::TWO):
        keysPressed[2] = false;
        break;
      case static_cast<int>(Keyboard::THREE):
        keysPressed[3] = false;
        break;
      case static_cast<int>(Keyboard::FOUR):
        keysPressed[4] = false;
        break;
      case static_cast<int>(Keyboard::FIVE):
        keysPressed[5] = false;
        break;
      case static_cast<int>(Keyboard::SIX):
        keysPressed[6] = false;
        break;
      case static_cast<int>(Keyboard::SEVEN):
        keysPressed[7] = false;
        break;
      case static_cast<int>(Keyboard::EIGHT):
        keysPressed[8] = false;
        break;
      case static_cast<int>(Keyboard::NINE):
        keysPressed[9] = false;
        break;
      case static_cast<int>(Keyboard::A):
        keysPressed[0xA] = false;
        break;
      case static_cast<int>(Keyboard::B):
        keysPressed[0xB] = false;
        break;
      case static_cast<int>(Keyboard::C):
        keysPressed[0xC] = false;
        break;
      case static_cast<int>(Keyboard::D):
        keysPressed[0xD] = false;
        break;
      case static_cast<int>(Keyboard::E):
        keysPressed[0xE] = false;
        break;
      case static_cast<int>(Keyboard::F):
        keysPressed[0xF] = false;
        break;
      default:
        break;
      }
      break;
    }
  }
}

void Display::endFrame() {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (screen[x][y] != 0) {
        SDL_Rect temp = {x * 10, y * 10, 10, 10};
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderFillRect(rend, &temp);
      }
    }
  }

  SDL_RenderPresent(rend);
  endTime = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
  if (diff < 16667) {
    std::this_thread::sleep_for(std::chrono::microseconds(16667 - diff));
  }
}

int Display::xorAtPoint(int x, int y, int value) {
  int old = screen[x][y];
  screen[x][y] ^= value;
  if (old == 1 && screen[x][y] != 1) {
    return 1;
  }
  return 0;
}

bool Display::getKeyPress(int key) { return keysPressed[key]; }

int Display::getAnyKey() {
  for (int i = 0; i <= 0xF; i++) {
    if (keysPressed[i]) {
      return i;
    }
  }
  return -1;
}