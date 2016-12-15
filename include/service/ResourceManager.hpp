#pragma once

#include <string>
#include <regex>

#include "collection/Option.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "collection/mutable/KBMap.hpp"
#include "resource/Loader.hpp"
#include "exception/ResourceException.hpp"

class ResourceManager {
public:
  ResourceManager() {}
  virtual ~ResourceManager() {}

  virtual auto addLoader(std::regex regex, std::shared_ptr<Loader> loader) -> void = 0;

  virtual auto load(std::string filePath) -> void = 0;

  template <typename T>
  auto get(const std::string& filePath) const -> Option<T> {
    auto m = _resources.get<T>().get();

    auto resource = m.get(filePath);

    if (resource.isDefined()) {
      return Option<T>(std::dynamic_pointer_cast<T>(resource.get()));
    } else {
      return Option<T>();
    }
  }

  template <typename T>
  auto getRequired(const std::string& filePath) const -> std::shared_ptr<T> {
    auto m = _resources.get<T>().get();

    auto resource = m.get(filePath);

    if (resource.isDefined()) {
      return std::dynamic_pointer_cast<T>(resource.get());
    } else {
      throw ResourceException("Required resource not found: " + filePath);
    }
  }

protected:
  KBTypeMap<KBMap<std::string, std::shared_ptr<Resource>>> _resources;
private:
  ResourceManager(const ResourceManager& resourceManager) {}
};


class NullResourceManager : public ResourceManager {
public:
  NullResourceManager() {}

  auto addLoader(std::regex regex, std::shared_ptr<Loader> loader) -> void override { }

  auto load(std::string filePath) -> void override { }
};
