#include <iostream>

#include "monolith/console/Console.hpp"

namespace monolith::app {
void Main() {
  CommandFactory factory(nullptr);
  Console console(factory);
  factory.SetConsole(&console);
  console.StartProcess();
  }
}

int main() {
  monolith::app::Main();
}