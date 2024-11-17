#ifndef MONOLITH_IOTHPTWRIGHT_HPP
#define MONOLITH_IOTHPTWRIGHT_HPP

#include "../console/Command.hpp"
#include <string>
#include <vector>

class IOThptWrite{
public:
  IOThptWrite();

  static void Run();
  void Run(const std::vector<std::string>& args);
  static void WriteToFile(std::string& file_path, size_t block_size_bytes, size_t total_size_bytes);
};

#endif  // MONOLITH_IOTHPTWRIGHT_HPP


