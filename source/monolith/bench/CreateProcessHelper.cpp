#include "CreateProcessHelper.hpp"

#include <windows.h>

#include <iostream>

int CreateProcessHelper::execute_process(const std::string& executable, const std::string& args) {
  std::string command_line = executable + " " + args;

  STARTUPINFOA si = { sizeof(STARTUPINFOA) };
  PROCESS_INFORMATION pi = {};

  if (!CreateProcessA(
          nullptr,
          command_line.data(),
          nullptr,
          nullptr,
          FALSE,
          0,
          nullptr,
          nullptr,
          &si,
          &pi
      )) {
    std::cerr << "Failed to create process. Error: " << GetLastError() << "\n";
    return -1;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code;
  if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
    std::cerr << "Failed to get exit code. Error: " << GetLastError() << "\n";
    return -1;
  }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return static_cast<int>(exit_code);
}
