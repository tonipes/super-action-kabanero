#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "service/MessagePublisher.hpp"
#include <SFML/Window.hpp>
#include "message/event/GameInputEvent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "service/Services.hpp"
#include "minebombers/events/NewGameEvent.hpp"

/**
 * InputTranslator class.
 * Translates SFML events to meaningful game input events.
 */
class InputTranslator {
private:

  std::map< sf::Keyboard::Key, std::tuple<GameInputAction, std::string>> keymap {
    {sf::Keyboard::W,       std::make_tuple(UP,           "gameScene:world/player1")},
    {sf::Keyboard::A,       std::make_tuple(LEFT,         "gameScene:world/player1")},
    {sf::Keyboard::S,       std::make_tuple(DOWN,         "gameScene:world/player1")},
    {sf::Keyboard::D,       std::make_tuple(RIGHT,        "gameScene:world/player1")},
    {sf::Keyboard::Num1,    std::make_tuple(FIRE_UP,      "gameScene:world/player1")},
    {sf::Keyboard::Num2,    std::make_tuple(FIRE_LEFT,    "gameScene:world/player1")},
    {sf::Keyboard::Num3,    std::make_tuple(FIRE,         "gameScene:world/player1")},

    {sf::Keyboard::Y,       std::make_tuple(UP,           "gameScene:world/player2")},
    {sf::Keyboard::G,       std::make_tuple(LEFT,         "gameScene:world/player2")},
    {sf::Keyboard::H,       std::make_tuple(DOWN,         "gameScene:world/player2")},
    {sf::Keyboard::J,       std::make_tuple(RIGHT,        "gameScene:world/player2")},
    {sf::Keyboard::Num5,    std::make_tuple(FIRE_UP,      "gameScene:world/player2")},
    {sf::Keyboard::Num6,    std::make_tuple(FIRE_LEFT,    "gameScene:world/player2")},
    {sf::Keyboard::Num7,    std::make_tuple(FIRE,         "gameScene:world/player2")},

    {sf::Keyboard::Up,      std::make_tuple(UP,           "gameScene:world/player3")},
    {sf::Keyboard::Left,    std::make_tuple(LEFT,         "gameScene:world/player3")},
    {sf::Keyboard::Down,    std::make_tuple(DOWN,         "gameScene:world/player3")},
    {sf::Keyboard::Right,   std::make_tuple(RIGHT,        "gameScene:world/player3")},
    {sf::Keyboard::Delete,  std::make_tuple(FIRE_UP,      "gameScene:world/player3")},
    {sf::Keyboard::End,     std::make_tuple(FIRE_LEFT,    "gameScene:world/player3")},
    {sf::Keyboard::PageDown, std::make_tuple(FIRE,        "gameScene:world/player3")},

    {sf::Keyboard::Numpad5,       std::make_tuple(UP,        "gameScene:world/player4")},
    {sf::Keyboard::Numpad1,       std::make_tuple(LEFT,      "gameScene:world/player4")},
    {sf::Keyboard::Numpad2,       std::make_tuple(DOWN,      "gameScene:world/player4")},
    {sf::Keyboard::Numpad3,       std::make_tuple(RIGHT,     "gameScene:world/player4")},
    {sf::Keyboard::Numpad7,    std::make_tuple(FIRE_UP,      "gameScene:world/player4")},
    {sf::Keyboard::Numpad8,    std::make_tuple(FIRE_LEFT,    "gameScene:world/player4")},
    {sf::Keyboard::Numpad9,    std::make_tuple(FIRE,         "gameScene:world/player4")}
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
    } if(event.key.code == sf::Keyboard::Escape && event.type == sf::Event::KeyPressed){
      Services::logger()->info("Change scene to main menu");
      Services::messagePublisher()->sendMessage(Message(
        "game",
        std::make_shared<NewGameEvent>(15, 0)
        )
      );
    }
  }

};
