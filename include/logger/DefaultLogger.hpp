#pragma once

#include <string>
#include <iostream>

#include "collection/mutable/KBMap.hpp"
#include "service/Logger.hpp"

/**
 * DefaultLogger class.
 * @todo Figure out a better way to define colors and headers
 */
class DefaultLogger: public Logger {
public:
  DefaultLogger(): level(DEBUG) {}
  ~DefaultLogger() {}

  auto setLevel(const LogLevel l) -> void { level = l; }

  auto debug(const std::string& message) -> void { msg(message, DEBUG); }
  auto info(const std::string& message) -> void { msg(message, INFO); }
  auto warn(const std::string& message) -> void { msg(message, WARN); }
  auto error(const std::string& message) -> void { msg(message, ERROR); }
  auto fatal(const std::string& message) -> void { msg(message, FATAL); }

protected:
  LogLevel level;

  bool enableColor = true;
  std::string colorReset = "\u001B[0m";

  std::map<LogLevel, std::string> colors {
    {DEBUG, "\u001B[32m"},
    {INFO, "\u001B[34m"},
    {WARN, "\u001B[33m"},
    {ERROR, "\u001B[31m"},
    {FATAL, "\u001B[35m"}
  };

  std::map<LogLevel, std::string> headers {
    {DEBUG, " DEBUG "},
    {INFO, " INFO  "},
    {WARN, " WARN  "},
    {ERROR, " ERROR "},
    {FATAL, " FATAL "}
  };

private:
  auto msg(const std::string& message, LogLevel l) -> void {
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
