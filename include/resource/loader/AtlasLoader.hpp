#pragma once

#include <regex>
#include <fstream>
#include <sstream>
#include <glm/vec2.hpp>
#include "resource/Loader.hpp"
#include "resource/resource/sprite/Sprite.hpp"
#include "resource/resource/sprite/Atlas.hpp"
#include "collection/mutable/KBMap.hpp"
#include "exception/ResourceException.hpp"

class AtlasLoader : public Loader {
public:
  AtlasLoader() :
    imageRegex("^.+\\.png$"),
    sizeRegex("^\\s*size:\\s*([0-9]+)\\s*,\\s*([0-9]+)$"),
    xyRegex("^\\s*xy:\\s*([0-9]+)\\s*,\\s*([0-9]+)$"),
    origRegex("^\\s*orig:\\s*([0-9]+)\\s*,\\s*([0-9]+)$"),
    offsetRegex("^\\s*offset:\\s*([0-9]+)\\s*,\\s*([0-9]+)$"),
    indexRegex("^\\s*index:\\s*(-?[0-9]+)\\s*$"),
    colonRegex(".+:.*") { }

  auto load(const std::string& filePath) -> const std::shared_ptr<Resource> override;
private:
  std::regex imageRegex;
  std::regex sizeRegex;
  std::regex xyRegex;
  std::regex origRegex;
  std::regex offsetRegex;
  std::regex indexRegex;
  std::regex colonRegex;
};
