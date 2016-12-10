#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"
#include "service/Services.hpp"
#include "resource/resource/Font.hpp"
#include "minebombers/data/HudParameters.hpp"

class HudEffect : public Effect {
public:
  HudEffect(int hp, std::string name) : Effect(1.0f), _hp(hp), _name(name) {
    auto resourceManager = Services::resourceManager();
    const auto& font = *Services::resourceManager()->getRequired<Font>("resources/font.ttf");

    text.setString(_name + "\n" + std::to_string(_hp));

    text.setFont(font.getFont());

    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    target.draw(text, states);
  }

  sf::Text text;
  int _hp;
  std::string _name;

};
