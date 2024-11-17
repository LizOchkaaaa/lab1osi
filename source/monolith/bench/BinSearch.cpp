// BinarySearch.cpp
#include "BinSearch.hpp"

#include <fstream>
#include <iostream>
#include <string>

std::vector<int> LoadArrayFromFile(const std::string& filename) {
  std::vector<int> array;
  std::ifstream in(filename, std::ios::binary);
  if (!in) {
    std::cerr << "Failed to open file for reading: " << filename << "\n";
    exit(1);
  }
  int value;
  while (in.read(reinterpret_cast<char*>(&value), sizeof(value))) {
    array.push_back(value);
  }
  in.close();
  return array;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: BinSearch.exe <target> <array_file>\n";
    return 1;
  }


  int target = std::stoi(argv[1]);
  std::string array_file = argv[2];


  std::vector<int> sorted_array = LoadArrayFromFile(array_file);

  int result = BinSearch::search(sorted_array, target);

  return result;
}
int BinSearch::search(const std::vector<int>& arr, int target) {
  int left = 0;
  int right = arr.size() - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;


    if (arr[mid] == target) {
      return mid;
    }


    if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}
