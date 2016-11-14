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

class ResourceManager {
public:
  ResourceManager() {}
  ~ResourceManager() {}

  auto addLoader(std::regex regex, std::shared_ptr<Loader> loader) -> void {
    _loaders += { regex, loader };
  }

  auto load(std::string filePath) -> void {
    for(auto pair : _loaders) {
      std::regex regex;
      std::shared_ptr<Loader> loader;
      std::tie(regex, loader) = pair;
      bool match = std::regex_match(filePath, regex);
      if (match){
        auto res = loader->load(filePath);
        _resources[typeid(*res)][filePath] = res;
        return;
      }
    }
    throw ResourceException("No matching loader found for file: " + filePath);
  }

  friend std::ostream& operator<<(std::ostream& os, ResourceManager resman);

  template <typename T>
  auto get(const std::string& filePath) const -> T& {
    KBMap<std::string, std::shared_ptr<Resource>> m = _resources.get<T>();

    auto resource = m[filePath];
    return *std::dynamic_pointer_cast<T>(resource);
  }

private:
  ResourceManager(const ResourceManager& resourceManager) {}

  KBVector<std::tuple<std::regex, std::shared_ptr<Loader>>> _loaders;

  KBTypeMap<KBMap<std::string, std::shared_ptr<Resource>>> _resources;

};

std::ostream& operator<<(std::ostream& os, ResourceManager resman) {
  os << "ResourceManager( " << resman._resources.length() << " )";
  return os;
}
