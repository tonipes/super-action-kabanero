#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"
#include "service/Services.hpp"
#include "resource/resource/Font.hpp"

/**
 * Draws a circle
 */
class TextEffect : public Effect {
public:
  TextEffect(std::string str, float time = 1.0f) : Effect(time), _str(str) {
    const auto& font = Services::resourceManager()->getRequired<Font>("resources/font.ttf");

    text.setString(_str);

    text.setFont(font->getFont());

    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    text.setOrigin(0, 0);
    text.setPosition(_x, _y);

    target.draw(text, states);
    text.setColor(sf::Color(255, 255, 255, 255 * _time));
  }

private:
  mutable sf::Text text;
  std::string _str;
};
