#pragma once
#include "Display.hpp"

using usix = uint16_t;
using uate = uint8_t;

class Interpreter {
protected:
  uate *memory;

  uate *V;
  usix *I;

  uate *delay;
  uate *sound;

  usix *programCounter;
  uate *stackPointer;

  usix *stack;

  Display *display;

protected:
  Interpreter(uate *memory, uate *V, usix *I, uate *delay, uate *sound,
              usix *programCounter, uate *stackPointer, usix *stack,
              Display *display);
  ~Interpreter();
public:
  // 0
  void sys0nnn();
  void cls00E0();
  void ret00EE();
  // 1
  void jp1nnn(usix nnn);
  // 2
  void call2nnn(usix nnn);
  // 3
  void se3xkk(uate Vx, uate kk);
  // 4
  void sne4xkk(uate Vx, uate kk);
  // 5
  void se5xy0(uate Vx, uate Vy);
  // 6
  void ld6xkk(uate Vx, uate kk);
  // 7
  void add7xkk(uate Vx, uate kk);
  // 8
  void ld8xy0(uate Vx, uate Vy);
  void or8xy1(uate Vx, uate Vy);
  void and8xy2(uate Vx, uate Vy);
  void xor8xy3(uate Vx, uate Vy);
  void add8xy4(uate Vx, uate Vy);
  void sub8xy5();
  void shr8xy6();
  void subn8xy7();
  void shl8xyE();
  // 9
  void sne9xy0();
  // A
  void ldAnnn();
  // B
  void jpBnnn();
  // C
  void rndCxkk();
  // D
  void drwDxyn();
  // E
  void skpEx9E();
  void sknpExA1();
  // F
  void ldFx07();



};