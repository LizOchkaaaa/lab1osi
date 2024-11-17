#ifndef HELP_HPP
#define HELP_HPP

#include "monolith/console/Console.hpp"  // Подключаем консоль для доступа к её методам
#include "monolith/console/Command.hpp"  // Подключаем базовый класс Command

class Help : public Command {
public:
  explicit Help(Console* console) : Command(console) {
  }
  void Run() override;
  void Run(const std::vector<std::string>& args) override;
};

#endif // HELP_HPP
