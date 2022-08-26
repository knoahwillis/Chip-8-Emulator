#pragma once

#include "Interpreter.hpp"
#include <cstdint>

class Chip {
protected:
  uate memory[4096]; //

  uate V[16]; // 8-bit registers
  usix I;     // generally stores memory addresses

  uate delay; // delay timer
  uate sound; // sound timer

  usix programCounter; // stores the address of the current program
  uate stackPointer;   //

  usix stack[16]; // stores the addresses that the interpreter shoudl return to
                  // after finishing a subroutine
  Display *display;

public:
  Chip();
  ~Chip();

public:
  void run();

protected:
  void handleIns();
};