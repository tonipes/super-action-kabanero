#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/Effect.hpp"
#include "resource/resource/Font.hpp"

class MainMenuEffect : public Effect {
public:
  MainMenuEffect(std::vector<std::string> choices, std::string motd, int currentChoise = 0) :
    Effect(currentChoise * 0.1f), _choices(choices), _currentChoise(currentChoise), _motd(motd) {
    const auto& font = Services::resourceManager()->getRequired<Font>("resources/font.ttf");
    title.setString("Super Action Kabanero");
    title.setFont(font->getFont());
    title.setStyle(sf::Text::Bold);
    title.setCharacterSize(72);
//    title.setOutlineColor(sf::Color::Black);
//    title.setOutlineThickness(5);

    subtitle.setString(_motd);
    subtitle.setFont(font->getFont());
    subtitle.setCharacterSize(32);
//    subtitle.setOutlineColor(sf::Color::Black);
//    subtitle.setOutlineThickness(5);

    title.setOrigin(title.getLocalBounds().width * 0.5f, title.getLocalBounds().height * 0.5f);
    for (auto a : _choices){
      sf::Text c;
      c.setString(a);
      c.setFont(font->getFont());
      c.setCharacterSize(32);
//      c.setOutlineColor(sf::Color::Black);
//      c.setOutlineThickness(5);
      _choicesText.push_back(c);
    }
  }

  auto draw(sf::RenderTarget &target, sf::RenderStates states) const -> void override {
    auto textBounds = title.getLocalBounds();
    auto viewCenter = target.getView().getCenter();

    title.setPosition(viewCenter.x, viewCenter.y * 0.33f);

    subtitle.setPosition(
      title.getGlobalBounds().left,
      title.getGlobalBounds().top + title.getGlobalBounds().height
    );

    target.draw(title, states);
    target.draw(subtitle, states);

    auto x = subtitle.getGlobalBounds().left;
    auto y = subtitle.getGlobalBounds().top + subtitle.getGlobalBounds().height + 50.0f;

    for(auto i = 0; i < _choicesText.size(); i++){

      auto a = _choicesText[i];
      if(i == _currentChoise){
        a.setColor(sf::Color::Red);
      } else {
        a.setColor(sf::Color::White);
      }

      a.setPosition(x, y);
      target.draw(a, states);
      x = a.getGlobalBounds().left;
      y = a.getGlobalBounds().top + a.getGlobalBounds().height;
    }

  }

private:
  // auto getChoice() const -> int {
  //   return (int) _time  / 0.1f;
  // }

  int _currentChoise;
  std::string _motd;
  mutable sf::Text title;
  mutable sf::Text subtitle;
  mutable std::vector<std::string> _choices;
  mutable std::vector<sf::Text> _choicesText;
};
