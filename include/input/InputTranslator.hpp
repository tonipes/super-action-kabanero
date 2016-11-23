#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "service/MessagePublisher.hpp"
#include <SFML/Window.hpp>
#include "message/event/GameInputEvent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "logger/DefaultLogger.hpp"




/**
 * DefaultMessagePublisher class.
 */
class InputTranslator {

public:
  InputTranslator() {}
  ~InputTranslator() {}

  auto processMessage(sf::Event event) -> void {
    //for testing
    auto logger = std::make_shared<DefaultLogger>();

        if(event.type == sf::Event::KeyPressed) {
          switch(event.key.code) {
            case sf::Keyboard::F : {
              logger->debug("PRESSED F");
              Services::messagePublisher()->sendMessage(Message("game:123", std::make_shared<GameInputEvent>(USE, true)));
            }
            break;
            case sf::Keyboard::W : {
              logger->debug("PRESSED W");
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(UP, true)));

            }
            break;
            case sf::Keyboard::A : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(LEFT, true)));
            }
            break;
            case sf::Keyboard::S : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(DOWN, true)));
            }
            break;
            case sf::Keyboard::D : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(RIGHT, true)));
            }
            break;
            case sf::Keyboard::Up : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(UP, true)));
            }
            break;
            case sf::Keyboard::Space : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(JUMP, true)));
            }
            break;
            default:
              break;
          }
        } else if(event.type == sf::Event::KeyReleased) {
          switch(event.key.code) {
            case sf::Keyboard::F : {
              Services::messagePublisher()->sendMessage(Message("game:123", std::make_shared<GameInputEvent>(USE, false)));
            }
            break;
            case sf::Keyboard::W : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(UP, false)));

            }
            break;
            case sf::Keyboard::A : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(LEFT, false)));
            }
            break;
            case sf::Keyboard::S : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(DOWN, false)));
            }
            break;
            case sf::Keyboard::D : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(RIGHT, false)));
            }
            break;
            case sf::Keyboard::Up : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(UP, false)));
            }
            break;
            case sf::Keyboard::Space : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(JUMP, false)));
            }
            break;
            default:
              break;
          }
        } else if(event.type == sf::Event::MouseButtonPressed) {
          switch(event.key.code) {
            case sf::Mouse::Left : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(FIRE, true)));
            }
            break;
            case sf::Mouse::Right : {
              Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(FIRE1, true)));
            }
            break;
            default:
            break;
          }
        } else if(event.type == sf::Event::MouseButtonReleased) {
            switch(event.key.code) {
              case sf::Mouse::Left : {
                Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(FIRE, false)));
              }
              break;
              case sf::Mouse::Right : {
                Services::messagePublisher()->sendMessage(Message("", std::make_shared<GameInputEvent>(FIRE1, false)));
              }
              break;
              default:
              break;
            }
        }
      }
};
