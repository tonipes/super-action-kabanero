#pragma once

#include <exception>
#include <string>

class EngineException: public std::exception {
public:
  EngineException(std::string description): _description(description) {}

  virtual const char* what() const throw() {
    return _description.c_str();
  }

protected:
  std::string _description;
};
