#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "resource/Resource.hpp"

/**
 * Texture Resource.
 */
class Texture : public Resource {
public:
  Texture(sf::Texture texture) : _texture(texture) {}

  auto getTexture() const -> sf::Texture {return _texture;}

  friend std::ostream& operator<<(std::ostream& os, Texture rexture);

private:
  const sf::Texture _texture;
};

std::ostream& operator<<(std::ostream& os, Texture rexture) {
  os << "Texture( " << " )";
  return os;
}
