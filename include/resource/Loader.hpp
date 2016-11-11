#pragma once

#include <memory>
#include <string>
#include <future>
#include <thread>

#include "resource/Resource.hpp"
#include "collection/Future.hpp"

/**
 * Resource loader interface.
 */
class Loader {
public:
  virtual auto load(const std::string& filePath) -> const std::shared_ptr<Resource> = 0;
};
