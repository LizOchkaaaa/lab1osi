#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>
#include <stack>
#include <vector>
#include "CommandFactory.hpp"  // Подключаем фабрику команд

class CommandFactory;
class Console {
public:

  explicit Console(CommandFactory& factory);


  void StartProcess();


  void Run();


  std::string ReadNextCommandString();


  void SetRunning(bool running) { running_ = running; }


  bool IsRunning() const { return running_; }

  std::stack<std::string> GetHistory() const { return history_; }


  std::string GetWorkingDirectory() const { return working_directory_; }
  void UpdateWorkingDirectory(const std::string& new_directory);

  std::stack<std::string> previous_directories_;

private:
  std::string working_directory_;
  std::stack<std::string> history_;

  void ExecuteCommand(const std::vector<char*>& args);

  bool running_ = true;
  CommandFactory& factory_;
};

#endif // CONSOLE_HPP
