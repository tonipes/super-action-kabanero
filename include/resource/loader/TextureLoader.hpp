#pragma once

#include <SFML/Graphics.hpp>

#include "resource/Loader.hpp"
#include "resource/resource/Texture.hpp"
#include "exception/ResourceException.hpp"

/**
 * Texture resource loader class.
 */
class TextureLoader : public Loader {
  auto load(const std::string& filePath) -> const std::shared_ptr<Resource> {
    sf::Texture texture;
    if (!texture.loadFromFile(filePath)){
      throw ResourceException("Couldn't load texture resource from file " + filePath);
    }
    return std::make_shared<Texture>(texture);
  }
};
