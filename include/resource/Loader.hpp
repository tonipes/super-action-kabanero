#pragma once

#include <memory>
#include <string>
#include <future>
#include <thread>

#include "resource/Resource.hpp"
#include "collection/Future.hpp"

class Loader {
public:
  typedef std::future<std::tuple<
      std::string,
      std::shared_ptr<Resource>
    >> FutureResource;
  auto loadAsync(const std::string& filePath) -> Future<std::shared_ptr<Resource>> {
    auto f = Future<std::shared_ptr<Resource>>([=]() {
      return load(filePath);
    });
    return f;
  }
protected:
  virtual auto load(const std::string& filePath) -> const std::shared_ptr<Resource> = 0;
};
