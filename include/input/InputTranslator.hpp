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

public:
  KBVector<std::string> paths;
  InputTranslator() {
    Services::logger()->debug("Create Input Translator");
    paths += "gameScene:world/player";
    paths += "gameScene:world/menu";
  }
  ~InputTranslator() {}

  auto sendInputEvent(std::shared_ptr<GameInputEvent> ev) {
    for (auto path : paths) {
      Services::messagePublisher()->sendMessage(Message(path, ev));
    }
  }

  /*This method processes the event and sends information to Services::messagepublisher,
  * the same message must be sent when pressing button and releasing the same button.
  */

  auto processMessage(sf::Event event) -> void {
    if(event.type == sf::Event::KeyPressed) {
      switch(event.key.code) {
        case sf::Keyboard::F : {
          sendInputEvent(std::make_shared<GameInputEvent>(USE, true));
        }
        break;
        case sf::Keyboard::W : {
          sendInputEvent(std::make_shared<GameInputEvent>(UP, true));
        }
        break;
        case sf::Keyboard::A : {
          sendInputEvent(std::make_shared<GameInputEvent>(LEFT, true));
        }
        break;
        case sf::Keyboard::S : {
          sendInputEvent(std::make_shared<GameInputEvent>(DOWN, true));
        }
        break;
        case sf::Keyboard::D : {
          sendInputEvent(std::make_shared<GameInputEvent>(RIGHT, true));
        }
        break;
        case sf::Keyboard::Space : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE, true));
        }
        break;
        case sf::Keyboard::Up : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_UP, true));
        }
        break;
        case sf::Keyboard::Down : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_DOWN, true));
        }
        break;
        case sf::Keyboard::Left : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_LEFT, true));
        }
        break;
        case sf::Keyboard::Right : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_RIGHT, true));
        }
        break;
        default:
          break;
      }
    } else if(event.type == sf::Event::KeyReleased) {
      switch(event.key.code) {
        case sf::Keyboard::F : {
          sendInputEvent(std::make_shared<GameInputEvent>(USE, false));
        }
        break;
        case sf::Keyboard::W : {
          sendInputEvent(std::make_shared<GameInputEvent>(UP, false));
        }
        break;
        case sf::Keyboard::A : {
          sendInputEvent(std::make_shared<GameInputEvent>(LEFT, false));
        }
        break;
        case sf::Keyboard::S : {
          sendInputEvent(std::make_shared<GameInputEvent>(DOWN, false));
        }
        break;
        case sf::Keyboard::D : {
          sendInputEvent(std::make_shared<GameInputEvent>(RIGHT, false));
        }
        break;
        case sf::Keyboard::Space : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE, false));
        }
        case sf::Keyboard::Up : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_UP, false));
        }
        break;
        case sf::Keyboard::Down : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_DOWN, false));
        }
        break;
        case sf::Keyboard::Left : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_LEFT, false));
        }
        break;
        case sf::Keyboard::Right : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE_RIGHT, false));
        }
        break;
        break;
        default:
          break;
      }
    } else if(event.type == sf::Event::MouseButtonPressed) {
      switch(event.key.code) {
        case sf::Mouse::Left : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE, true));
        }
        break;
        case sf::Mouse::Right : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE1, true));
        }
        break;
        default:
        break;
      }
    } else if(event.type == sf::Event::MouseButtonReleased) {
      switch(event.key.code) {
        case sf::Mouse::Left : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE, false));
        }
        break;
        case sf::Mouse::Right : {
          sendInputEvent(std::make_shared<GameInputEvent>(FIRE1, false));
        }
        break;
        default:
        break;
      }
    }
  }
};
