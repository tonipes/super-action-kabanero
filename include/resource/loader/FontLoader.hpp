#pragma once

#include <SFML/Graphics.hpp>

#include "resource/Loader.hpp"
#include "resource/resource/Font.hpp"
#include "exception/ResourceException.hpp"

/**
 * Font resource loader class.
 */
class FontLoader : public Loader {
  auto load(const std::string& filePath) -> const std::shared_ptr<Resource> {
    sf::Font font;
    if (!font.loadFromFile(filePath)){
      throw ResourceException("Couldn't load font resource from file " + filePath);
    }
    return std::make_shared<Font>(font);
  }
};
