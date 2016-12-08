#pragma once

#include "minebombers/ui/MainMenu.hpp"
#include "minebombers/ui/ButtonSelectedEvent.hpp"

class MenuBehaviour : public Behavior<Transform3D> {
public:
  int selDir = 0;
  int index;
  bool press = false;
  MenuBehaviour(Node<Transform3D>* node) : index(0) {
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();

      if (action == UP && isPressed) {
        selDir = -1;
      } else if (action == DOWN && isPressed) {
        selDir = 1;
      } else if (action == FIRE && isPressed) {
        press = true;
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
   if (selDir != 0 || press) {
     index += selDir;
     if (index >= 3) index = 0;
     if (index < 0) index = 2;
     selDir = 0;
     Services::messagePublisher()->sendMessage(Message(
       "all",
       std::make_shared<ButtonSelectedEvent>(index, press)
     ));
     press = false;
   }
  }
};
