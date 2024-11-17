#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "BinSearch.hpp"
#include "CreateProcessHelper.hpp"
#define MAX_VAL 100000

void print_usage() {
  std::cerr << "Usage: " << "./Bin" << " <iterations>" << '\n';
}

void save_array_to_file(const std::vector<int>& array, const std::string& filename) {
  std::ofstream out(filename, std::ios::binary);
  if (!out) {
    std::cerr << "Failed to open file for writing: " << filename << "\n";
    exit(1);
  }
  for (int val : array) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
  }
  out.close();
}

  int main(int argc, char* argv[]) {
    std::vector<int> sorted_array;
    std::vector<double> durations;

    if (argc != 2) {
      print_usage();
      return 1;
    }
    int iterations = std::stoi(argv[1]);

    for (int i = 1; i <= MAX_VAL; i++) {
      sorted_array.push_back(i);
    }

      std::string array_file = "array.bin";
      save_array_to_file(sorted_array, array_file);

    int target;

    for (int i = 0; i < iterations; i++) {
      auto start = std::chrono::high_resolution_clock::now();

      target = rand() % MAX_VAL + 1;
      int result = CreateProcessHelper::execute_process(
          "BinSearch.exe", std::to_string(target) + " " + array_file
      );
      std::cout << result << "\n";
      auto end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double, std::milli> duration = end - start;
      durations.push_back(duration.count());
    }

    double avg_duration =
        std::accumulate(durations.begin(), durations.end(), 0.0) / durations.size();
    double min_duration = *std::min_element(durations.begin(), durations.end());
    double max_duration = *std::max_element(durations.begin(), durations.end());

    std::cout << "\nOverall Statistics:\n";
    std::cout << "Average execution time: " << avg_duration << " milli seconds\n";
    std::cout << "Minimum execution time: " << min_duration << " milli seconds\n";
    std::cout << "Maximum execution time " << max_duration << " milli seconds\n";

    return 0;
  }
