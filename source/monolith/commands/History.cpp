#include "History.hpp"
#include <iostream>

void History::Run() {
  if (console_ == nullptr) {
    std::cerr << "Error: Console not initialized.\n";
    return;
  }

  auto history = console_->GetHistory();
  if (history.empty()) {
    std::cout << "History is empty." << std::endl;
    return;
  }

  std::cout << "Command History:" << std::endl;
  int index = 1;
  while (!history.empty()) {
    std::cout << index++ << ": " << history.top() << std::endl;
    history.pop();
  }
}

void History::Run(const std::vector<std::string>& args) {
  if (args.empty()) {
    Run();
    return;
  }

  std::cout << "Executing history command with arguments:\n";
  for (const auto& arg : args) {
    std::cout << "Argument: " << arg << "\n";
  }
  Run();
}
