#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "resource/Resource.hpp"

/**
 * Font Resource.
 */
class Font : public Resource {
public:
  Font(sf::Font font) : _font(font) {}

  auto getFont() const -> const sf::Font& {return _font;}

  // friend std::ostream& operator<<(std::ostream& os, Font font);

private:
  const sf::Font _font;
};
