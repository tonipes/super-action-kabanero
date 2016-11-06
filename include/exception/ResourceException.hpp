#pragma once

#include "exception/EngineException.hpp"

class ResourceException: public EngineException {
public:
  ResourceException(std::string description): EngineException(description) {}

};
