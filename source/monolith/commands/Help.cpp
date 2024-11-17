#include "Help.hpp"
#include <iostream>
#include <map>

const std::map<std::string, std::string> CommandDescription = {
    {"help", "Displays a list of available commands"},
    {"exit", "Exits the program"},
    {"cat", "Reads a file"},
    {"cd", "Changes to the specified directory"},
    {"history", "Displays command history"},
    {"ls", "Lists files in the current directory"},
    {"mkdir", "Creates a new directory"},
    {"rm", "Removes a directory"},
};


void Help::Run() {
}

void Help::Run(const std::vector<std::string>& args) {
  if (!args.empty()) {
    std::string command = args[0];
    auto it = CommandDescription.find(command);
    if (it != CommandDescription.end()) {
      std::cout << "Usage: " << it->first << "\n" << it->second << "\n";
    } else {
      std::cout << "Error: Command '" << command << "' not found.\n";
    }
  } else {
    std::cout << "Available commands:\n";
    for (const auto& command : CommandDescription) {
      std::cout << "  " << command.first << ": " << command.second << "\n";
    }
  }
}
