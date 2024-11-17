#include "Console.hpp"
#include "CommandFactory.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <io.h>
#include <windows.h>

std::pair<std::string, int> pwd() {
  char cwd[1024];
  if (_getcwd(cwd, sizeof(cwd)) != nullptr) {
    return {std::string(cwd), 0};
  } else {
    perror("pwd");
    return {"", 1};
  }
}


std::vector<std::string> SplitString(const std::string& line) {
  std::vector<std::string> tokens;
  std::istringstream stream(line);
  std::string token;

  while (stream >> token) {
    tokens.push_back(token);
  }

  return tokens;
}

void Console::StartProcess() {
  std::cout << "Input command " << std::endl;
  auto [pwd_string, pwd_status_code] = pwd();
  if (pwd_status_code != 0) {
    std::cout << "Failed to initialize working directory." << std::endl;
    return;
  }
  std::cout << "Current working directory is: " << pwd_string << std::endl;
  working_directory_ = pwd_string;
  this->Run();
}

void Console::Run() {
  while (this->running_) {
    std::cout << "[" + working_directory_ + "]> ";
    std::string command_string = ReadNextCommandString();
    if (command_string.empty()) {
      continue;
    }

    auto token_list = SplitString(command_string);
    if (token_list.empty()) {
      continue;
    }

    std::string command_name = token_list[0];
    std::vector<std::string> args(token_list.begin() + 1, token_list.end());

    auto command = factory_.CreateCommand(command_name, args);

    auto start = std::chrono::high_resolution_clock::now();

    if (command != nullptr) {
      command->Run();
    } else {
      std::vector<char*> c_args;
      for (const auto& arg : args) {
        c_args.push_back(const_cast<char*>(arg.c_str()));
      }
      c_args.push_back(nullptr);

      ExecuteCommand(c_args);
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;


    std::cout << "Command executed in: " << elapsed.count() << " seconds" << std::endl;

    this->history_.push(command_string);
  }
}

void Console::ExecuteCommand(const std::vector<char*>& args) {
  std::string command;
  for (const auto& arg : args) {
    if (arg != nullptr) {
      command += arg;
      command += " ";
    }
  }

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  std::cout << "Executing command: " << command << std::endl;

  if (!CreateProcess(nullptr, const_cast<char*>(command.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
    std::cerr << "CreateProcessHelper failed (" << GetLastError() << ")." << std::endl;
    return;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

std::string Console::ReadNextCommandString() {
  std::string command_string;
  std::getline(std::cin, command_string);
  return command_string;
}

Console::Console(CommandFactory& factory) : factory_(factory) {}
