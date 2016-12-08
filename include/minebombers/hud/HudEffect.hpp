#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"
#include "service/Services.hpp"
#include "resource/resource/Font.hpp"

class HudEffect : public Effect {
public:
  HudEffect(std::string name) : Effect(1.0f), _name(name) {
    auto resourceManager = Services::resourceManager();
    const auto& font = *Services::resourceManager()->getRequired<Font>("resources/font.ttf");

    text.setFont(font.getFont());
    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    text.setString(_string);
    target.draw(text, states);

  }

  auto update(int hp) -> void {
    _string = _name + "\n" + "HP: " + std::to_string(hp);
  }

private:
  sf::Font font;
  std::string _name;
  std::string _string;
  mutable sf::Text text;
};
