#include "Interpreter.hpp"
#include <ctime>
#include <random>

Interpreter::Interpreter(uate* memory, uate* V, usix* I, uate* delay, uate* sound, usix* programCounter, uate* stackPointer, usix* stack, Display* display) {
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

void Interpreter::sys0nnn() {}

void Interpreter::cls00E0() { display->clear(); }

void Interpreter::ret00EE() {
  *programCounter = *(stack + *stackPointer);
  *stackPointer--;
}

void Interpreter::jp1nnn(usix nnn) { *programCounter = nnn - 2; }

void Interpreter::call2nnn(usix nnn) {
  if (*stackPointer == 0xF) {
    *stackPointer = 0x0;
  } else {
    *stackPointer++;
  }
  *(stack + *stackPointer) = *programCounter;
  *programCounter = nnn - 2;
}

void Interpreter::se3xkk(uate Vx, uate kk) {
  if (*(V + Vx) == kk) {
    *programCounter += 2;
  }
}

void Interpreter::sne4xkk(uate Vx, uate kk) {
  if (*(V + Vx) != kk) {
    *programCounter += 2;
  }
}

void Interpreter::se5xy0(uate Vx, uate Vy) {
  if (*(V + Vx) == *(V + Vy)) {
    *programCounter += 2;
  }
}

void Interpreter::ld6xkk(uate Vx, uate kk) { *(V + Vx) = kk; }

void Interpreter::add7xkk(uate Vx, uate kk) { *(V + Vx) += kk; }

void Interpreter::ld8xy0(uate Vx, uate Vy) { *(V + Vx) = *(V + Vy); }

void Interpreter::or8xy1(uate Vx, uate Vy) { *(V + Vx) = *(V + Vx) | *(V + Vy); }

void Interpreter::and8xy2(uate Vx, uate Vy) { *(V + Vx) = *(V + Vx) & *(V + Vy); }

void Interpreter::xor8xy3(uate Vx, uate Vy) { *(V + Vx) = *(V + Vx) ^ *(V + Vy); }

void Interpreter::add8xy4(uate Vx, uate Vy) {
  if (Vx + Vy > 0xFF) {
    *(V + 0xF) = 1;
    *(V + Vx) = 0xFF;
  } else {
    *(V + Vx) += *(V + Vy);
  }
}

void Interpreter::sub8xy5(uate Vx, uate Vy) {
  if (*(V + Vx) > *(V + Vy)) {
    *(V + 0xF) = 1;
  } else {
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
  if (*(V + Vy) > *(V + Vx)) {
    *(V + 0xF) = 1;
  } else {
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
  if (*(V + Vx) != *(V + Vy)) {
    *programCounter += 2;
  }
}

void Interpreter::ldAnnn(usix nnn) { *I = nnn; }

void Interpreter::jpBnnn(usix nnn) { *programCounter = nnn + *V; }

void Interpreter::rndCxkk(uate Vx, uate kk) {
  srand(time(0));
  uate rando = rand() % 255;
  *(V + Vx) = rando & kk;
}

void Interpreter::drwDxyn(uate Vx, uate Vy, uate nnn) {
  int x = *(V + Vx) + 7;
  int y = *(V + Vy);
  for (uate i = 0; i < nnn; i++) {
    uate curr = *(memory + *I + i);
    for (int j = 0; j < 8; j++) {
      int val = (curr >> j) & 0x1;
      int cX = x;
      int cY = y;
      if (cX > 64) {
        cX -= 64;
      } else if (cX < 0) {
        cX += 64;
      }
      if (cY > 32) {
        cY -= 32;
      } else if (cY < 0) {
        cY += 32;
      }
      display->xorAtPoint(cX, cY, val);
      x--;
    }
    x = *(V + Vx) + 7;
    y++;
  }
}

void Interpreter::skpEx9E(uate Vx) {
  if (display->getKeyPress(static_cast<int>(*(V + Vx)))) {
    *programCounter += 2;
  }
}

void Interpreter::sknpExA1(uate Vx) {
  if (!display->getKeyPress(static_cast<int>(*(V + Vx)))) {
    *programCounter += 2;
  }
}

void Interpreter::ldFx07(uate Vx) { *(V + Vx) = *delay; }

void Interpreter::ldFx0A(uate Vx) {
  int check = display->getAnyKey();
  if (check == -1) {
    *programCounter -= 2;
  } else {
    *(V + Vx) = check;
  }
}

void Interpreter::ldFx15(uate Vx) { *delay = *(V + Vx); }

void Interpreter::ldFx18(uate Vx) { *sound = *(V + Vx); }

void Interpreter::addFx1E(uate Vx) { *I += *(V + Vx); }

void Interpreter::ldFx29(uate Vx) { *I = *(V + Vx) * 5; }

void Interpreter::ldFx33(uate Vx) {
  int num = static_cast<int>(*(V + Vx));
  int hundreds = (num / 100) % 10;
  int tens = (num / 10) % 10;
  int ones = num % 10;

  *(memory + *I) = hundreds;
  *(memory + *I + 1) = tens;
  *(memory + *I + 2) = ones;
}

void Interpreter::ldFx55(uate Vx) {
  for (uate i = 0; i <= Vx; i++) {
    *(memory + *I + i) = *(V + i);
  }
}

void Interpreter::ldFx65(uate Vx) {
  for (uate i = 0; i <= Vx; i++) {
    *(V + i) = *(memory + *I + i);
  }
}
