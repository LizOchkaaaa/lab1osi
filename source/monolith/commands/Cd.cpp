#include "Cd.hpp"
#include <iostream>
#include <filesystem>

void Cd::Run() {

}

void Cd::Run(const std::vector<std::string>& args) {
  if (args.empty()) {
    Run();
    return;
  }

  const std::string& new_directory = args[0];
  std::filesystem::path current_path = std::filesystem::current_path();

  try {
    if (new_directory == "..") {
      console_->previous_directories_.push(current_path.string());
      std::filesystem::current_path(current_path.parent_path());
    } else if (new_directory == "-") {

      if (!console_->previous_directories_.empty()) {
        std::string previous_directory = console_->previous_directories_.top();
        console_->previous_directories_.pop();
        std::filesystem::current_path(previous_directory);
      } else {
        std::cerr << "Error: No previous directory to return to.\n";
        return;
      }
    } else if (std::filesystem::exists(new_directory) && std::filesystem::is_directory(new_directory)) {
      console_->previous_directories_.push(current_path.string());
      std::filesystem::current_path(new_directory);
    } else {
      std::cerr << "Error: Directory does not exist or is not accessible: " << new_directory << "\n";
      return;
    }
    console_->UpdateWorkingDirectory(std::filesystem::current_path().string());
    std::cout << "Changed directory to: " << std::filesystem::current_path() << "\n";
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

void Console::UpdateWorkingDirectory(const std::string& new_directory) {
  working_directory_ = new_directory;
  std::cout << "Working directory updated to: " << working_directory_ << std::endl;
}
