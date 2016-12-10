#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "service/MessagePublisher.hpp"
#include <SFML/Window.hpp>
#include "message/event/GameInputEvent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "service/Services.hpp"

class InputTranslator {
private:

  std::map< sf::Keyboard::Key, std::tuple<GameInputAction, std::string>> keymap {
    {sf::Keyboard::W,       std::make_tuple(UP,           "gameScene:world/player1")},
    {sf::Keyboard::A,       std::make_tuple(LEFT,         "gameScene:world/player1")},
    {sf::Keyboard::S,       std::make_tuple(DOWN,         "gameScene:world/player1")},
    {sf::Keyboard::D,       std::make_tuple(RIGHT,        "gameScene:world/player1")},

    {sf::Keyboard::Y,       std::make_tuple(FIRE_UP,      "gameScene:world/player1")},
    {sf::Keyboard::G,       std::make_tuple(FIRE_LEFT,    "gameScene:world/player1")},
    {sf::Keyboard::H,       std::make_tuple(FIRE_DOWN,    "gameScene:world/player1")},
    {sf::Keyboard::J,       std::make_tuple(FIRE_RIGHT,   "gameScene:world/player1")},

    {sf::Keyboard::Up,      std::make_tuple(UP,           "gameScene:world/player2")},
    {sf::Keyboard::Left,    std::make_tuple(LEFT,         "gameScene:world/player2")},
    {sf::Keyboard::Down,    std::make_tuple(DOWN,         "gameScene:world/player2")},
    {sf::Keyboard::Right,   std::make_tuple(RIGHT,        "gameScene:world/player2")},

    {sf::Keyboard::Numpad8, std::make_tuple(FIRE_UP,      "gameScene:world/player2")},
    {sf::Keyboard::Numpad4, std::make_tuple(FIRE_LEFT,    "gameScene:world/player2")},
    {sf::Keyboard::Numpad5, std::make_tuple(FIRE_DOWN,    "gameScene:world/player2")},
    {sf::Keyboard::Numpad6, std::make_tuple(FIRE_RIGHT,   "gameScene:world/player2")},

    {sf::Keyboard::Num1,    std::make_tuple(NUM_1,        "gameScene")},
    {sf::Keyboard::Num2,    std::make_tuple(NUM_2,        "gameScene")},
    {sf::Keyboard::Num3,    std::make_tuple(NUM_3,        "gameScene")},
    {sf::Keyboard::Num4,    std::make_tuple(NUM_4,        "gameScene")}
  };

public:
  InputTranslator() {
    Services::logger()->debug("Create Input Translator");
  }
  ~InputTranslator() {}

  /*This method processes the event and sends information to Services::messagepublisher,
  * the same message must be sent when pressing button and releasing the same button.
  */
  auto processMessage(sf::Event event) -> void {
    if(keymap.find(event.key.code) != keymap.end()){
      auto a = keymap[event.key.code];
      auto pressed = event.type == sf::Event::KeyPressed;
      Services::messagePublisher()->sendMessage(
        Message(std::get<1>(a), std::make_shared<GameInputEvent>(std::get<0>(a), pressed)
      ));
    }
  }
  
};
