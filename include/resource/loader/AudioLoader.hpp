#pragma once

#include <SFML/Audio.hpp>

#include "resource/Loader.hpp"
#include "resource/resource/Audio.hpp"
#include "exception/ResourceException.hpp"

/**
 * Audio resource loader class.
 */
class AudioLoader : public Loader {
  auto load(const std::string& filePath) -> const std::shared_ptr<Resource> {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath)){
      throw ResourceException("Couldn't load audio resource from file " + filePath);
    }
    return std::make_shared<Audio>(buffer);
  }
};
