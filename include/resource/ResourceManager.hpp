#pragma once

#include <string>
#include <regex>
#include <tuple>
#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "collection/Option.hpp"

#include "resource/Loader.hpp"

class ResourceManager {
public:
  typedef Future<std::shared_ptr<Resource>> FutureResource;

  auto addLoader(std::regex regex, std::shared_ptr<Loader> loader) -> void {
    _loaders += { regex, loader };
  }

  auto load(std::string filePath) -> void {
    auto loaderOption = _loaders.find([&](const auto& pair) {
      std::regex r;
      std::shared_ptr<Loader> l;
      std::tie(r, l) = pair;
      return std::regex_match(filePath, r);
    }).map([&](const auto& pair) {
      std::regex r;
      std::shared_ptr<Loader> l;
      std::tie(r, l) = pair;
      return l;
    });
    if (loaderOption.isEmpty()) {
      // throw error
    }
    auto loader = loaderOption.get();

    _futureResources += { filePath, loader->loadAsync(filePath) };
    _numToLoad += 1;
  }

  template <typename T>
  auto get(const std::string& filePath) const -> Option<T> {
    Option<KBMap<std::string, std::shared_ptr<Resource>>> m = _resources.get<T>();

    return m.map([&](KBMap<std::string, std::shared_ptr<Resource>> resourceMap) {
      Option<std::shared_ptr<Resource>> resource = resourceMap.get(filePath);
      return *std::dynamic_pointer_cast<T>(resource.get());
    });
  }

  auto update() -> void {
    _futureResources.remove([&](auto& futurePair) {
      auto filePath = std::get<0>(futurePair);
      auto& future = std::get<1>(futurePair);
      if (future.isReady()) {
        auto value = future.get();
        _resources[typeid(*value)][filePath] = value;
        _numLoaded += 1;
        return true;
      }
      return false;
    });
    // Once ready, reset values
    if (isReady()) {
      _numLoaded = 0;
      _numToLoad = 0;
    }
  }

  // Return value between 0.0 and 1.0
  auto getProgress() -> float {
    if (isReady()) {
      return 1.0;
    } else {
      return float(_numToLoad) / _numLoaded;
    }
  }

  auto isReady() -> bool {
    return _futureResources.isEmpty();
  }


private:
  KBVector<std::tuple<std::regex, std::shared_ptr<Loader>>> _loaders;

  KBTypeMap<KBMap<std::string, std::shared_ptr<Resource>>> _resources;

  KBVector<std::tuple<std::string, FutureResource>> _futureResources;

  int _numToLoad = 0;
  int _numLoaded = 0;
};
