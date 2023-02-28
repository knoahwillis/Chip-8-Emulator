#pragma once

#include "Interpreter.hpp"
#include <cstdint>
#include <string>

class Chip {
  protected:
  uate memory[0xFFF + 1]; //

  uate V[16]; // 8-bit registers
  usix I;     // generally stores memory addresses

  uate delay; // delay timer
  uate sound; // sound timer

  usix programCounter; // stores the address of the current program
  uate stackPointer;   //

  usix stack[16]; // stores the addresses that the interpreter shoudl return to
                  // after finishing a subroutine
  Display* display;
  Interpreter* interpreter;

  bool running;

  public:
  Chip(std::string rom);
  ~Chip();

  void run();

  protected:
  void loadRom(std::string rom, int start);
  void handleIns(uate Vx, uate Vy);

  void handle0(usix bytes[4]);
  void handle1(usix bytes[4]);
  void handle2(usix bytes[4]);
  void handle3(usix bytes[4]);
  void handle4(usix bytes[4]);
  void handle5(usix bytes[4]);
  void handle6(usix bytes[4]);
  void handle7(usix bytes[4]);
  void handle8(usix bytes[4]);
  void handle9(usix bytes[4]);
  void handleA(usix bytes[4]);
  void handleB(usix bytes[4]);
  void handleC(usix bytes[4]);
  void handleD(usix bytes[4]);
  void handleE(usix bytes[4]);
  void handleF(usix bytes[4]);

  bool compBytes(usix bytes[4], std::string instr);
  usix c2u(char c);
};