#ifndef MONOLITH_CD_HPP
#define MONOLITH_CD_HPP

#include "monolith/console/Console.hpp"
#include "monolith/console/Command.hpp"
#include <vector>
#include <string>
#include <stack>

class Cd : public Command {
public:
  explicit Cd(Console* console) : Command(console) {}

  void Run() override;
  void Run(const std::vector<std::string>& args) override;


};

#endif  // MONOLITH_CD_HPP
