#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

#include "CreateProcessHelper.hpp"


int main(int argc, char* argv[]) {
  int iterations;
  std::vector<double> durations;
  std::string args = argv[2];
  for (int i = 3; i < argc; i++) {
    args += " ";
    args += argv[i];

  }
  try {
    iterations = std::stoi(argv[1]);
    if (iterations <= 0) {
      throw std::invalid_argument("Iterations must be positive.");
    }
  } catch (const std::exception& e) {
    std::cerr << "Invalid iterations value: " << args[1] << '\n';
    return 1;
  }
  char* file_path = argv[2];
  size_t total_size_mb = std::stoul(argv[4]);
  size_t total_size_bytes = total_size_mb * 1024 * 1024;
  for (int i = 0; i < iterations; ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int result = CreateProcessHelper::execute_process("IOThptWright.exe", args);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    durations.push_back(duration.count());

    double throughput = total_size_bytes / (duration.count() * 1024 * 1024);
    std::cout << "Written " << total_size_mb << " MB to " << file_path << " in " << duration.count()
              << " seconds. Throughput: " << throughput << " MB/s\n";
  }

  double avg_duration = std::accumulate(durations.begin(), durations.end(), 0.0) / durations.size();
  double min_duration = *std::min_element(durations.begin(), durations.end());
  double max_duration = *std::max_element(durations.begin(), durations.end());

  std::cout << "\nOverall Statistics:\n";
  std::cout << "Average write time: " << avg_duration << " seconds\n";
  std::cout << "Minimum write time: " << min_duration << " seconds\n";
  std::cout << "Maximum write time: " << max_duration << " seconds\n";
  return 0;
}
