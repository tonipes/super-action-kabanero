#pragma once

#include <string>
#include <iostream>

#include "collection/mutable/KBMap.hpp"
#include "logger/Logger.hpp"

/**
 * DefaultLogger class.
 * @todo Figure out a better way to define colors and headers
 */
class DefaultLogger: public Logger {
public:
  DefaultLogger() {}
  ~DefaultLogger() {}

private:
  auto msg(const std::string& message, LogLevel l) -> void override {
    if(l >= level){
      if(enableColor){
        printConsole(colors[l] + "[" + headers[l] + "] " + message + "\u001B[0m");
      } else {
        printConsole("[" + headers[l] + "] " + message + "\u001B[0m");
      }
    }
  }

  auto printConsole(const std::string& line) -> void {
    std::cout << line << std::endl;
  }
};
