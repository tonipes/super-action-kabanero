#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

/**
 * Effect class.
 * Used for graphical effects and other drawing that needs access to SFML RenderTarget.
 */
class Effect : public sf::Drawable {
public:
  Effect(float time): _time(time), _x(0), _y(0) {}

  virtual auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void = 0;

  auto time() -> float {
    return _time;
  }

  auto setPosition(float x, float y) -> void {
    _x = x;
    _y = y;
  }

  auto setTileSize(float size) -> void {
    tileSize = size;
  }

protected:
  float _time;
  float _x, _y;
  float tileSize;
};
