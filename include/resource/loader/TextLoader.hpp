#pragma once

#include "resource/Loader.hpp"
#include "resource/resource/Text.hpp"
#include <fstream>
#include <iterator>

class TextLoader : public Loader {
protected:
  auto load(const std::string& filePath) -> const std::shared_ptr<Resource> {
    std::ifstream in { filePath };
    std::string resource = {
      std::istreambuf_iterator<char>(in),
      std::istreambuf_iterator<char>()
    };
    return std::make_shared<Text>(resource);
  }
};
