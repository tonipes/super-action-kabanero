#pragma once

#include "resource/Loader.hpp"
#include "resource/resource/Text.hpp"
#include "exception/ResourceException.hpp"
#include <fstream>
#include <iterator>

class TextLoader : public Loader {
public:
  auto load(const std::string& filePath) -> const std::shared_ptr<Resource> {
    std::ifstream in { filePath };
    if(!in.is_open()) {
      throw ResourceException("Couldn't load file: " + filePath);
    }
    std::string resource = {
      std::istreambuf_iterator<char>(in),
      std::istreambuf_iterator<char>()
    };
    resource.pop_back(); // Last line end
    return std::make_shared<Text>(resource);
  }
};
