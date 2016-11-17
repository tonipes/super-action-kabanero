#pragma once

#include <string>
#include <regex>
#include <tuple>
#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "collection/Option.hpp"
#include "exception/ResourceException.hpp"
#include "resource/ResourceManager.hpp"

#include "resource/Loader.hpp"

/**
 * SyncResourceManager class.
 */
class SyncResourceManager: public ResourceManager {
public:
  SyncResourceManager() {}
  ~SyncResourceManager() {}

  auto addLoader(std::regex regex, std::shared_ptr<Loader> loader) -> void override {
    _loaders += { regex, loader };
  }

  auto load(std::string filePath) -> void override {
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

private:
  KBVector<std::tuple<std::regex, std::shared_ptr<Loader>>> _loaders;
};
