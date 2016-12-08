#pragma once

#include "minebombers/ui/MainMenu.hpp"
#include "minebombers/ui/MenuController.hpp"

class MenuBehaviour : public Behavior<Transform3D> {
public:
  MenuBehaviour(Node<Transform3D>* node, MenuController& cont) {
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();

      if (action == UP && isPressed) {
        cont.changeSelection(-1);
      } else if (action == DOWN && isPressed) {
        cont.changeSelection(1);
      } else if (action == FIRE && isPressed) {
        cont.select();
      }
    });
  }
};
