#include <iostream>
#include <string>

std::string take_input() {
  std::string input;
  std::string line;

  while (std::getline(std::cin, line)) {
    input += line + "\n";
  }

  return input;
};

