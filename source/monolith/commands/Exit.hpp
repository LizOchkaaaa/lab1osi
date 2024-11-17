#ifndef EXIT_HPP
#define EXIT_HPP

#include "monolith/console/Console.hpp"  // Подключаем консоль для доступа к её методам
#include "monolith/console/Command.hpp"  // Подключаем базовый класс Command

class Exit : public Command {
public:
  explicit Exit(Console* console) : Command(console) {}

  void Run() override;
  void Run(const std::vector<std::string>& args) override;

private:
  Console* console_;
};

#endif // EXIT_HPP
