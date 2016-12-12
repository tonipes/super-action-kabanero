#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "graphics/effects/MainMenuEffect.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "minebombers/events/NewGameEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class MainMenuBehavior : public Behavior<Transform3D> {
public:
  MainMenuBehavior(Node<Transform3D>* node, std::vector<std::string> choices, std::string motd): _motd(motd), _choices(choices) {
    node->addEventReactor([&, node](GameInputEvent event) {
      Services::logger()->debug("Menu input event");
      auto action = event.action();
      auto isPressed = event.isPressed();

      if (action == UP) {
        moveUp = isPressed;
      } else if (action == DOWN) {
        moveDown = isPressed;
      } else if (action == RIGHT) {
        select = isPressed;
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if (moveUp || moveDown) {
      if(moveUp) currentChoice -= 1;
      if(moveDown) currentChoice += 1;

      currentChoice = currentChoice < 0 ? 0 : currentChoice;
      currentChoice = currentChoice >= _choices.size() ? _choices.size() - 1 : currentChoice;

      node.addAttachment(std::make_shared<EffectAttachment>(
        std::make_shared<MainMenuEffect>(_choices, _motd, currentChoice)
      ));

      Services::messagePublisher()->sendMessage(Message(
        "audioPlayer:clip/gunshot.ogg",
        std::make_shared<AudioClipEvent>(CLIP_PLAY)
      ));
    } else if (select){
      Services::messagePublisher()->sendMessage(Message(
        "game",
        std::make_shared<NewGameEvent>(15, currentChoice + 1)
        )
      );

      Services::messagePublisher()->sendMessage(Message(
        "audioPlayer:clip/reload.ogg",
        std::make_shared<AudioClipEvent>(CLIP_PLAY)
      ));
    }

    moveUp = false;
    moveDown = false;
    select = false;
  }

private:
  bool moveUp = false;
  bool moveDown = false;
  bool select = false;

  int currentChoice = 0;
  std::string _motd;

  std::vector<std::string> _choices;
  bool destroyed = false;
};
