#pragma once

#include <string>
#include <regex>
#include <tuple>
#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "collection/Option.hpp"
#include "exception/ResourceException.hpp"

#include "resource/Loader.hpp"

/**
* ResourceManager interface.
* @todo use Option class.
*/
class ResourceManager {
public:
  ResourceManager() {}
  virtual ~ResourceManager() {}

  virtual auto addLoader(std::regex regex, std::shared_ptr<Loader> loader) -> void = 0;

  virtual auto load(std::string filePath) -> void = 0;

  template <typename T>
  auto get(const std::string& filePath) const -> T& {
    KBMap<std::string, std::shared_ptr<Resource>> m = _resources.get<T>();

    auto resource = m[filePath];
    return *std::dynamic_pointer_cast<T>(resource);
  }

protected:
  KBTypeMap<KBMap<std::string, std::shared_ptr<Resource>>> _resources;

private:
  ResourceManager(const ResourceManager& resourceManager) {}
};
