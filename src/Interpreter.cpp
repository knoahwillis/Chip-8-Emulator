#include "Interpreter.hpp"
#include <random>
#include <ctime>

Interpreter::Interpreter(uate *memory, uate *V, usix *I, uate *delay,
                         uate *sound, usix *programCounter, uate *stackPointer,
                         usix *stack, Display *display) {
  this->memory = memory;
  this->V = V;
  this->I = I;
  this->delay = delay;
  this->sound = sound;
  this->programCounter = programCounter;
  this->stackPointer = stackPointer;
  this->stack = stack;
  this->display = display;
}

void Interpreter::sys0nnn() { ; }

void Interpreter::cls00E0() { display->clear(); }

void Interpreter::ret00EE() {
  *programCounter = *(stack + *stackPointer);
  *stackPointer--;
}

void Interpreter::jp1nnn(usix nnn) { *programCounter = nnn; }

void Interpreter::call2nnn(usix nnn) {
  if (*stackPointer == 0xF) {
    *stackPointer = 0x0;
  } else {
    *stackPointer++;
  }
  *(stack + *stackPointer) = *programCounter;
  *programCounter = nnn;
}

void Interpreter::se3xkk(uate Vx, uate kk) {
  if (*(V + Vx) == kk) {
    *programCounter += 2;
  }
}

void Interpreter::sne4xkk(uate Vx, uate kk) {
  if(*(V + Vx) != kk) {
    *programCounter += 2;
  }
}

void Interpreter::se5xy0(uate Vx, uate Vy) {
  if(*(V + Vx) == *(V + Vy)) {
    *programCounter += 2;
  }
}

void Interpreter::ld6xkk(uate Vx, uate kk) {
  *(V + Vx) = kk;
}

void Interpreter::add7xkk(uate Vx, uate kk) {
  *(V + Vx) += kk;
}

void Interpreter::ld8xy0(uate Vx, uate Vy) {
  *(V + Vx) = *(V + Vy);
}

void Interpreter::or8xy1(uate Vx, uate Vy) {
   *(V + Vx) = *(V + Vx) | *(V + Vy);
}

void Interpreter::and8xy2(uate Vx, uate Vy) {
  *(V + Vx) = *(V + Vx) & *(V + Vy);
}

void Interpreter::xor8xy3(uate Vx, uate Vy) {
  *(V + Vx) = *(V + Vx) ^ *(V + Vy);
}

void Interpreter::add8xy4(uate Vx, uate Vy) {
  if(Vx + Vy > 0xFF) {
    *(V + 0xF) = 1;
    *(V + Vx) = 0xFF;
  }else {
    *(V + Vx) += *(V + Vy);
  }
}

void Interpreter::sub8xy5(uate Vx, uate Vy) {
  if(*(V + Vx) > *(V + Vy)) {
    *(V + 0xF) = 1;
  }
  else {
    *(V + 0xF) = 0;
  }
  *(V + Vx) -= *(V + Vy);
}

void Interpreter::shr8xy6(uate Vx, uate Vy) {
  uate checker = *(V + Vx) & 0x1;
  *(V + 0xF) = checker;
  *(V + Vx) = *(V + Vx) / 2;
}

void Interpreter::subn8xy7(uate Vx, uate Vy) {
  if(*(V + Vy) > *(V + Vx)) {
    *(V + 0xF) = 1;
  }
  else {
    *(V + 0xF) = 0;
  }
  *(V + Vx) = *(V + Vy) - *(V + Vx);
}

void Interpreter::shl8xyE(uate Vx, uate Vy) {
  uate checker = (*(V + Vx) >> 7) & 0x1;
  *(V + 0xF) = checker;
  *(V + Vx) = *(V + Vx) * 2;
}

void Interpreter::sne9xy0(uate Vx, uate Vy) {
  if(*(V + Vx) != *(V + Vy)) {
    *programCounter += 2;
  }
}

void Interpreter::ldAnnn(usix nnn) {
  *I = nnn; 
}

void Interpreter::jpBnnn(usix nnn) {
  *programCounter = nnn + *V;
}

void Interpreter::rndCxkk(uate Vx, uate kk) {
  srand(time(0));
  uate rando = rand() % 255;
  *(V + Vx) = rando & kk; 
}

