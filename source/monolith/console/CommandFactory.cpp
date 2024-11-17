#include "CommandFactory.hpp"

#include <iostream>

#include "Console.hpp"
#include "monolith/Bench/BinSearch.hpp"
#include "monolith/Bench/IOThptWright.hpp"
#include "monolith/commands/Cat.hpp"
#include "monolith/commands/Cd.hpp"
#include "monolith/commands/Exit.hpp"
#include "monolith/commands/Help.hpp"
#include "monolith/commands/History.hpp"
#include "monolith/commands/Ls.hpp"
#include "monolith/commands/Mkdir.hpp"
#include "monolith/commands/Rm.hpp"

CommandFactory::CommandFactory(Console* console) : console_(console) {
  RegisterCommands();
}


void CommandFactory::RegisterCommands() {
  // Регистрируем команду ls
  command_map_["ls"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Ls>(console_);
    command->Run(args);
    return command;
  };

  // Регистрируем команду history
  command_map_["history"] = [this](std::vector<std::string> args) {
    return std::make_unique<History>(console_);
  };

  // Регистрируем команду help
  command_map_["help"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Help>(console_);
    command->Run(args);
    return command;
  };

  // Регистрация команды Exit
  command_map_["exit"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Exit>(console_);
    command->Run(args);
    return command;
  };

  // Регистрация команды Cat
  command_map_["cat"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Cat>(console_);
    command->Run(args);
    return command;
  };

  // Регистрация команды cd
  command_map_["cd"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Cd>(console_);
    command->Run(args);
    return command;
  };


  // Регистрация команды mkdir
  command_map_["mkdir"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Mkdir>(console_);
    command->Run(args);
    return command;
  };

  // Регистрация команды rm
  command_map_["rm"] = [this](std::vector<std::string> args) {
    auto command = std::make_unique<Rm>(console_);
    command->Run(args);
    return command;
  };
}

std::unique_ptr<Command> CommandFactory::CreateCommand(const std::string& command_name, std::vector<std::string> args) {
  auto it = command_map_.find(command_name);
  if (it != command_map_.end()) {
    return it->second(args);
  }
  return nullptr;
}
void CommandFactory::SetConsole(Console* console) {
  console_ = console;
}