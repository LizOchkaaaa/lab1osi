#include "Ls.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


Ls::Ls(Console* console) : Command(console) {}

void Ls::Run() {
  Run({});
}

void Ls::Run(const std::vector<std::string>& args) {
  fs::path dir_path = args.empty() ? fs::current_path() : args[0];

  if (fs::exists(dir_path) && fs::is_directory(dir_path)) {
    for (const auto& entry : fs::directory_iterator(dir_path)) {
      std::cout << entry.path().filename().string() << "\n";
    }
  } else {
    std::cerr << "Ошибка: директория " << dir_path << " не найдена или не является директорией.\n";
  }
}
