#include "Chip.hpp"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Not enough arguments! Need file path to ROM!" << std::endl;
    std::terminate();
  }

  std::string romPath(argv[1]);

  std::unique_ptr<Chip> emulator = std::make_unique<Chip>(romPath);
  emulator->run();

  return 0;
}