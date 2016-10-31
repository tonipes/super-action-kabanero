#pragma once

#include <memory>
#include <string>
#include <future>
#include <thread>

#include "resource/Resource.hpp"

class Loader {
public:
  Loader() {}

  auto loadAsync(const std::string& filePath) -> const std::shared_ptr<Resource> {
    // std::promise<std::shared_ptr<Resource>> p;
    // auto f = p.get_future();
    // std::thread work_thread(&Loader::load, filePath);
    // return f;
    // return std::move(std::async(std::launch::async, [=](){ return load(filePath); }));
    return load(filePath);
  }
protected:
  virtual auto load(const std::string& filePath) -> const std::shared_ptr<Resource> = 0;
};
