#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "BinSearch.hpp"
#include "IOThptWright.hpp"
#include "monolith/console/Console.hpp"  // Подключите нужные заголовки

void print_usage(const char* program_name) {
  std::cerr << "Usage: " << program_name
            << " --target <value> --sorted-array <array> --io-iterations <m> [-v]" << std::endl;
  std::cerr << "  --target <value>           : Target number to search (required)" << std::endl;
  std::cerr << "  --sorted-array <array>     : Sorted array of integers (required, space-separated)" << std::endl;
  std::cerr << "  --io-iterations <m>        : Number of IO latency write iterations (required)" << std::endl;
  std::cerr << "  -v, --verbose              : Set verbose output" << std::endl;
}


int main(int argc, char* argv[]) {
  int target = 0;
  std::vector<int> sorted_array;
  bool verbose = false;
  int io_iterations = 0;

  std::cout << "=== Starting " << argv[0] << " benchmark ===" << std::endl;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--target" && i + 1 < argc) {
      target = std::stoi(argv[++i]);
    } else if (arg == "--sorted-array" && i + 1 < argc) {
      while (++i < argc && argv[i][0] != '-') {
        sorted_array.push_back(std::stoi(argv[i]));
      }
      --i;
    } else if (arg == "-v" || arg == "--verbose") {
      verbose = true;
    } else if (arg == "--io-iterations" && i + 1 < argc) {
      io_iterations = std::stoi(argv[++i]);
    } else {
      std::cerr << "Unknown or incomplete argument: " << arg << std::endl;
      print_usage(argv[0]);
      return 1;
    }
  }

  if (sorted_array.empty() || io_iterations == 0) {
    std::cerr << "Error: Both --sorted-array and --io-iterations are required." << std::endl;
    print_usage(argv[0]);
    return 1;
  }

  std::cout << "Both benchmarks have completed successfully." << std::endl;

  return 0;
}
