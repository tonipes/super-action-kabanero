#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "resource/Resource.hpp"

/**
 * Audio Resource.
 */
class Audio : public Resource {
public:
  Audio(sf::SoundBuffer buffer) : _buffer(buffer) {
    _sound.setBuffer(_buffer);
  }

  auto getBuffer() const -> const sf::SoundBuffer& {
    return _buffer;
  }
  auto getSound() const -> const sf::Sound& {
    return _sound;
  }

  friend std::ostream& operator<<(std::ostream& os, Audio audio);

private:
  const sf::SoundBuffer _buffer;
  sf::Sound _sound;
};

std::ostream& operator<<(std::ostream& os, Audio audio) {
  os << "Audio( " << " )";
  return os;
}
