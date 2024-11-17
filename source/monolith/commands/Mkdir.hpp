#ifndef MONOLITH_MKDIR_HPP
#define MONOLITH_MKDIR_HPP
#include "monolith/console/Console.hpp"  // Подключаем консоль для доступа к её методам
#include "monolith/console/Command.hpp"  // Подключаем базовый класс Command
#include <string>
#include <vector>

class Mkdir : public Command {
public:
  explicit Mkdir(Console* console) : Command(console) {}
  void Run() override;
  void Run(const std::vector<std::string>& args) override;
};


#endif  // MONOLITH_MKDIR_HPP
