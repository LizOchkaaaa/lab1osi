#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Command.hpp"

class CommandFactory {
public:
  CommandFactory(Console* console);
  void SetConsole(Console* console);
  std::unique_ptr<Command> CreateCommand(const std::string& command_name, std::vector<std::string> args);

private:
  Console* console_;
  std::unordered_map<std::string, std::function<std::unique_ptr<Command>(std::vector<std::string>)>> command_map_;

  void RegisterCommands();
};

#endif // COMMAND_FACTORY_HPP
