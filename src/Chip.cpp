#include "Chip.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Chip::Chip(std::string rom) {

  display = new Display;
  interpreter =
      new Interpreter(&memory[0], &V[0], &I, &delay, &sound, &programCounter,
                      &stackPointer, &stack[0], display);

  delay = 0;
  sound = 0;

  stackPointer = static_cast<uate>(16);

  for(int i = 0; i <= 0xF; i++) {
    V[i] = 0;
  }

  uate font[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70,
                   0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0,
                   0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0,
                   0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40,
                   0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0,
                   0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
                   0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0,
                   0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80};

  for (int i = 0; i < 80; i++) {
    memory[i] = font[i];
  }

  loadRom(rom, 0x200);
  programCounter = 0x200;

  running = true;
}

Chip::~Chip() {
  delete display;
  delete interpreter;
}

void Chip::run() {
  while (running) {
    display->renderFrame();
  }
}

void Chip::loadRom(std::string rom, int start) {
  std::ifstream f(rom, std::ios_base::binary | std::ios_base::in);
  if(!f) {
    std::cout << "Could not load ROM " << rom;
    std::terminate();
  }
  
  std::stringstream s;
  s << f.rdbuf();
  std::string file = s.str();

  const char* mem = file.c_str();
  std::size_t size = file.size();
  if(size > 0xFFF) {
    std::cout << "ROM too large to read!";
    std::terminate();
  }

  for(std::size_t i = 0; i < size; i++) {
    *(memory + start + i) = mem[i];
  }

}