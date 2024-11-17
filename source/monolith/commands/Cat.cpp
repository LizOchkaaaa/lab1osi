#include "Cat.hpp"
#include <iostream>
#include <fstream>

void Cat::Run() {
}


void Cat::Run(const std::vector<std::string>& args) {
  if (args.empty()) {
    Run();
    return;
  }

  for (const auto& filename : args) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Error: Unable to open file " << filename << "\n";
      continue;
    }

    std::cout << "Contents of " << filename << ":\n";
    std::string line;
    while (std::getline(file, line)) {
      std::cout << line << "\n";
    }
    std::cout << "---- End of " << filename << " ----\n";
  }
}
