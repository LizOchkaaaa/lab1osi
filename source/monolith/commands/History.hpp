#ifndef MONOLITH_HISTORY_HPP
#define MONOLITH_HISTORY_HPP
#include "monolith/console/Console.hpp"  // Подключаем консоль для доступа к её методам
#include "monolith/console/Command.hpp"  // Подключаем базовый класс Command

class History : public Command {
public:
  explicit History(Console* console) : Command(console) {
  }
  void Run() override;
  void Run(const std::vector<std::string>& args) override;
};

#endif  // MONOLITH_HISTORY_HPP
