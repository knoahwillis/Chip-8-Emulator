#include "Chip.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Chip::Chip(std::string rom) {

  display = new Display(&running);
  interpreter = new Interpreter(&memory[0], &V[0], &I, &delay, &sound, &programCounter, &stackPointer, &stack[0], display);

  delay = 0;
  sound = 0;

  stackPointer = static_cast<uate>(16);

  for (int i = 0; i <= 0xF; i++) {
    V[i] = 0;
  }

  uate font[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0,
                   0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40,
                   0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
                   0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80};

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
    display->beginFrame();
    uate Vx = memory[programCounter];
    uate Vy = memory[programCounter + 1];

    handleIns(Vx, Vy);

    if (programCounter > 0xFFF) {
      running = false;
    }

    programCounter += 2;

    if (delay != 0) {
      delay -= 1;
    }
    if (sound != 0) {
      sound -= 1;
    }

    display->endFrame();
  }
}

void Chip::handleIns(uate Vx, uate Vy) {
  usix bytes[] = {static_cast<usix>((Vx >> 4) & 0xF), static_cast<usix>(Vx & 0xF), static_cast<usix>((Vy >> 4) & 0xF), static_cast<usix>(Vy & 0xF)};

  switch (bytes[0]) {
  case 0x0:
    handle0(bytes);
    break;
  case 0x1:
    handle1(bytes);
    break;
  case 0x2:
    handle2(bytes);
    break;
  case 0x3:
    handle3(bytes);
    break;
  case 0x4:
    handle4(bytes);
    break;
  case 0x5:
    handle5(bytes);
    break;
  case 0x6:
    handle6(bytes);
    break;
  case 0x7:
    handle7(bytes);
    break;
  case 0x8:
    handle8(bytes);
    break;
  case 0x9:
    handle9(bytes);
    break;
  case 0xA:
    handleA(bytes);
    break;
  case 0xB:
    handleB(bytes);
    break;
  case 0xC:
    handleC(bytes);
    break;
  case 0xD:
    handleD(bytes);
    break;
  case 0xE:
    handleE(bytes);
    break;
  case 0xF:
    handleF(bytes);
    break;
  default:
    std::cout << "Bad Opcode!" << std::endl;
    break;
  }
}

usix Chip::c2u(char c) {
  usix t = static_cast<usix>(c);
  if (c >= 48 && c <= 57)
    return t - 48;
  if (c >= 65 && c <= 70)
    return t - 55;
  return t;
}

bool Chip::compBytes(usix bytes[4], std::string instr) {
  if (instr.length() != 4) {
    return false;
  }
  const char* ins = instr.c_str();

  for (int i = 0; i < 4; i++) {
    if (c2u(ins[i]) != bytes[i]) {
      if (ins[i] != 'n' && ins[i] != 'x' && ins[i] != 'y' && ins[i] != 'k') {
        return false;
      }
    }
  }

  return true;
}

void Chip::handle0(usix bytes[4]) {
  if (compBytes(bytes, "00E0")) {
    interpreter->cls00E0();
  } else if (compBytes(bytes, "00EE")) {
    interpreter->ret00EE();
  } else if (compBytes(bytes, "0nnn")) {
    std::cout << "0nnn" << std::endl;
  }
}

void Chip::handle1(usix bytes[4]) {
  usix addr = ((bytes[1] & 0xF) << 8) + ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->jp1nnn(addr);
}

void Chip::handle2(usix bytes[4]) {
  usix addr = ((bytes[1] & 0xF) << 8) + ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->call2nnn(addr);
}

void Chip::handle3(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  usix b = ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->se3xkk(reg, b);
}

void Chip::handle4(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  usix b = ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->sne4xkk(reg, b);
}

void Chip::handle5(usix bytes[4]) {
  if (compBytes(bytes, "5xy0")) {
    usix reg1 = bytes[1] & 0xF;
    usix reg2 = bytes[2] & 0xF;
    interpreter->se5xy0(reg1, reg2);
  }
}

void Chip::handle6(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  usix b = ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->ld6xkk(reg, b);
}

void Chip::handle7(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  usix b = ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->add7xkk(reg, b);
}

void Chip::handle8(usix bytes[4]) {
  usix reg1 = bytes[1] & 0xF;
  usix reg2 = bytes[2] & 0xF;
  if (compBytes(bytes, "8xy0")) {
    interpreter->ld8xy0(reg1, reg2);
  } else if (compBytes(bytes, "8xy1")) {
    interpreter->or8xy1(reg1, reg2);
  } else if (compBytes(bytes, "8xy2")) {
    interpreter->and8xy2(reg1, reg2);
  } else if (compBytes(bytes, "8xy3")) {
    interpreter->xor8xy3(reg1, reg2);
  } else if (compBytes(bytes, "8xy4")) {
    interpreter->add8xy4(reg1, reg2);
  } else if (compBytes(bytes, "8xy5")) {
    interpreter->sub8xy5(reg1, reg2);
  } else if (compBytes(bytes, "8xy6")) {
    interpreter->shr8xy6(reg1, reg2);
  } else if (compBytes(bytes, "8xy7")) {
    interpreter->subn8xy7(reg1, reg2);
  } else if (compBytes(bytes, "8xyE")) {
    interpreter->shl8xyE(reg1, reg2);
  }
}

void Chip::handle9(usix bytes[4]) {
  if (compBytes(bytes, "9xy0")) {
    usix reg1 = bytes[1] & 0xF;
    usix reg2 = bytes[2] & 0xF;
    interpreter->sne9xy0(reg1, reg2);
  }
}

void Chip::handleA(usix bytes[4]) {
  usix addr = ((bytes[1] & 0xF) << 8) + ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->ldAnnn(addr);
}

void Chip::handleB(usix bytes[4]) {
  usix addr = ((bytes[1] & 0xF) << 8) + ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->jpBnnn(addr);
}

void Chip::handleC(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  usix b = ((bytes[2] & 0xF) << 4) + (bytes[3] & 0xF);
  interpreter->rndCxkk(reg, b);
}

void Chip::handleD(usix bytes[4]) {
  usix reg1 = bytes[1] & 0xF;
  usix reg2 = bytes[2] & 0xF;
  usix nibble = bytes[3] & 0xF;
  interpreter->drwDxyn(reg1, reg2, nibble);
}

void Chip::handleE(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  if (compBytes(bytes, "Ex9E")) {
    interpreter->skpEx9E(reg);
  } else if (compBytes(bytes, "ExA1")) {
    interpreter->sknpExA1(reg);
  }
}

void Chip::handleF(usix bytes[4]) {
  usix reg = bytes[1] & 0xF;
  if (compBytes(bytes, "Fx07")) {
    interpreter->ldFx07(reg);
  }
  if (compBytes(bytes, "Fx0A")) {
    interpreter->ldFx0A(reg);
  }
  if (compBytes(bytes, "Fx15")) {
    interpreter->ldFx15(reg);
  }
  if (compBytes(bytes, "Fx18")) {
    interpreter->ldFx18(reg);
  }
  if (compBytes(bytes, "Fx1E")) {
    interpreter->addFx1E(reg);
  }
  if (compBytes(bytes, "Fx29")) {
    interpreter->ldFx29(reg);
  }
  if (compBytes(bytes, "Fx33")) {
    interpreter->ldFx33(reg);
  }
  if (compBytes(bytes, "Fx55")) {
    interpreter->ldFx55(reg);
  }
  if (compBytes(bytes, "Fx65")) {
    interpreter->ldFx65(reg);
  }
}

void Chip::loadRom(std::string rom, int start) {
  std::ifstream f(rom, std::ios_base::binary | std::ios_base::in);
  if (!f) {
    std::cout << "Could not load ROM " << rom;
    std::terminate();
  }

  std::stringstream s;
  s << f.rdbuf();
  std::string file = s.str();

  const char* mem = file.c_str();
  std::size_t size = file.size();
  if (size > 0xFFF) {
    std::cout << "ROM too large to read!";
    std::terminate();
  }

  for (std::size_t i = 0; i < size; i++) {
    *(memory + start + i) = mem[i];
  }
}