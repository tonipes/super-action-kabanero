#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"

/**
 * Draws a circle
 */
class CircleEffect : public Effect {
public:
  CircleEffect(float radius, float time = 1.0f) : Effect(time), _radius(radius) {
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(5.0f);
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    circle.setRadius(_radius*tileSize);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(_x, _y);

    target.draw(circle, states);
  }

private:
  mutable sf::CircleShape circle;
  float _radius;
};
