#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"

class MainMenuEffect : public Effect {
public:
  MainMenuEffect() : Effect(1.0f), _radius(100.0f) {
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(5.0f);
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    circle.setRadius(_radius);
    auto size = target.getSize();
    auto view = target.getView();
    auto viewCenter = view.getCenter();
    auto viewport = target.getViewport(view);

    auto mapped = target.mapCoordsToPixel(viewCenter);

    circle.setOrigin(circle.getRadius(), circle.getRadius());
    Services::logger()->debug("size: " + std::to_string(size.x) +  ", " + std::to_string(size.y) + "viewport: " + std::to_string(viewport.width)+  ", " + std::to_string(viewport.height));

    // circle.setPosition(mapped.x / 2.0f, mapped.x / 2.0f);
    circle.setPosition(800, 600);

    target.draw(circle, states);
  }

private:
  mutable sf::CircleShape circle;
  float _radius;
};
