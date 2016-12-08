#pragma once

#include "minebombers/ui/MainMenu.hpp"
#include "minebombers/ui/ButtonSelectedEvent.hpp"
#include "minebombers/GameType.hpp"

class MenuButtonBehaviour : public Behavior<Transform3D> {
public:
  bool value;
  int index;

  bool setTo;
  GameType gameType;
  bool throwEvent = false;
  MenuButtonBehaviour(Node<Transform3D>* node, int i, bool v, GameType gt) : index(i), value(v), setTo(v), gameType(gt) {
    node->addEventReactor([&](ButtonSelectedEvent event) {
      if (index == event.index()) setTo = value;
      else setTo = !value;
      if (event.isPress()) {
        throwEvent = value;
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    node.setRenderOn(setTo);
    if (throwEvent) {
      throwEvent = false;
      Services::messagePublisher()->sendMessage(Message(
        "all",
        std::make_shared<GameTypeSelectedEvent>(gameType)
      ));
    }
  }
};
