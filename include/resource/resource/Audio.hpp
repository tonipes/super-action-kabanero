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
    // Just for testing. Actually should use AudioPlayer
    _sound.setBuffer(_buffer);
    _sound.play();
  }

  friend std::ostream& operator<<(std::ostream& os, Audio audio);

private:
  sf::SoundBuffer _buffer;
  sf::Sound _sound;
};

std::ostream& operator<<(std::ostream& os, Audio audio) {
  os << "Audio( " << " )";
  return os;
}
