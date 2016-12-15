#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"
#include "service/Services.hpp"
#include "resource/resource/Font.hpp"

class HudEffect : public Effect {
public:
  HudEffect(std::string str) : Effect(1.0f), _str(str) {
    const auto& font = Services::resourceManager()->getRequired<Font>("resources/font.ttf");

    text.setString(_str);

    text.setFont(font->getFont());

    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
//    text.setOutlineColor(sf::Color::Black);
//    text.setOutlineThickness(2.0f);
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    auto viewCenter = target.getView().getCenter();
    auto viewSize = target.getView().getSize();

    text.setPosition(viewCenter.x - viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);

    target.draw(text, states);
  }

  mutable sf::Text text;
  std::string _str;

};
