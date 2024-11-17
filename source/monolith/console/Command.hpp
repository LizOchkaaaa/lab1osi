#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <vector>
#include <string>
#include <memory>

class Console;
class Command {
protected:
  Console* console_;

public:
  Command() = default;

  Command(Console* console) : console_(console) {}

  virtual void Run() = 0;
  virtual void BindToShell(Console* console) { console_ = console; }

  virtual ~Command() = default;
  virtual void Run(const std::vector<std::string>& args) = 0;
};

#endif // COMMAND_HPP

