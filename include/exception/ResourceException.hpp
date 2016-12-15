#pragma once

#include "exception/EngineException.hpp"

/**
 * Resource loading related exception
 */
class ResourceException: public EngineException {
public:
  ResourceException(std::string description): EngineException(description) {}

};
