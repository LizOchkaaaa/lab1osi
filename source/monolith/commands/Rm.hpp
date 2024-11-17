#ifndef MONOLITH_RM_HPP
#define MONOLITH_RM_HPP
#include "monolith/console/Console.hpp"  // Подключаем консоль для доступа к её методам
#include "monolith/console/Command.hpp"  // Подключаем базовый класс Command
#include <string>
#include <vector>

class Rm : public Command {
public:
  explicit Rm(Console* console) : Command(console) {}
  void Run() override;
  void Run(const std::vector<std::string>& args) override;
};

#endif  // MONOLITH_RM_HPP

