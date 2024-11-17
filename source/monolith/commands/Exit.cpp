#include "Exit.hpp"
#include <iostream>

void Exit::Run() {
  if (this->console_ == nullptr) {
    std::cout << "This command requires a shell to run." << std::endl;
    return;
  }

  std::cout << "Exiting shell..." << std::endl;


  console_->SetRunning(false);
}


void Exit::Run(const std::vector<std::string>& /* args */) {
  Run();
}
