#ifndef MONOLITH_CREATEPROCESSHELPER_H
#define MONOLITH_CREATEPROCESSHELPER_H

#include <string>

class CreateProcessHelper {
public:
  static int execute_process(const std::string& executable, const std::string& args);
};

#endif  // MONOLITH_CREATEPROCESSHELPER_H
