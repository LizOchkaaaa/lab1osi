#include "IOThptWright.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>


void IOThptWrite::WriteToFile(std::string& file_path, size_t block_size_bytes, size_t total_size_bytes){
  std::ofstream output_file(file_path, std::ios::binary);
  if (!output_file) {
    std::cerr << "Error: Cannot create file " << file_path << "\n";
    return;
  }
  std::vector<char> buffer(block_size_bytes, 'A');
  size_t total_blocks = total_size_bytes / block_size_bytes;

  for (size_t i = 0; i < total_blocks; ++i) {
    output_file.write(buffer.data(), buffer.size());
    if (!output_file) {
      std::cerr << "Error: Writing to file failed at block " << i << "\n";
      output_file.close();
      return;
    }
  }
  output_file.flush();
  output_file.close();
}

void IOThptWrite::Run() {
  std::cout << "Usage: io-thpt-write <file_path> <block_size_kb> <total_size_mb>\n";
}

 void IOThptWrite::Run(const std::vector<std::string>& args) {
  if (args.size() != 3) {
    Run();
    return;
  }

  std::string file_path = args[0];
  size_t block_size_kb = 0;
  size_t total_size_mb = 0;


  try {
    block_size_kb = std::stoul(args[1]);
    total_size_mb = std::stoul(args[2]);
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: Block size and total size must be numeric values.\n";
    return;
  } catch (const std::out_of_range& e) {
    std::cerr << "Error: Block size or total size is too large.\n";
    return;
  }
  size_t block_size_bytes = block_size_kb * 1024;
  size_t total_size_bytes = total_size_mb * 1024 * 1024;

  if (block_size_bytes == 0 || total_size_bytes == 0 || total_size_bytes % block_size_bytes != 0) {
    std::cerr << "Error: Invalid parameters. Ensure block size and total size are non-zero and compatible.\n";
    return;
  }

    IOThptWrite::WriteToFile(file_path, block_size_bytes, total_size_bytes);

}
IOThptWrite::IOThptWrite() {
}

int main(int argc, char* argv[]){
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.emplace_back(argv[i]);
  }
  IOThptWrite benchmark;
  benchmark.Run(args);
  return 0;
}
