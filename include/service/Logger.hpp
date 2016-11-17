#pragma once

#include <string>
#include <iostream>

#include "collection/mutable/KBMap.hpp"

enum LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3,
  FATAL = 4
};

/**
 * Logger interface.
 */
class Logger {
public:
  Logger() {}
  virtual ~Logger() {}

  virtual auto setLevel(const LogLevel l) -> void = 0;

  virtual auto debug(const std::string& message) -> void = 0;
  virtual auto info(const std::string& message) -> void = 0;
  virtual auto warn(const std::string& message) -> void = 0;
  virtual auto error(const std::string& message) -> void = 0;
  virtual auto fatal(const std::string& message) -> void = 0;

private:
  Logger(Logger& logger) {}

};

class NullLogger {
  NullLogger() {}
  auto setLevel(const LogLevel l) -> void { }

  auto debug(const std::string& message) -> void { }
  auto info(const std::string& message) -> void { }
  auto warn(const std::string& message) -> void { }
  auto error(const std::string& message) -> void { }
  auto fatal(const std::string& message) -> void { }

};
