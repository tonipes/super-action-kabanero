#pragma once

#include "minebombers/ui/MainMenu.hpp"

class MenuBehaviour : public Behavior<Transform3D> {
public:
  MenuBehaviour(Node<Transform3D>* node, MainMenu& main) {
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();

      if (action == UP) {
        main.changeSelection(-1);
      } else if (action == DOWN) {
        main.changeSelection(1);
      } else if (action == FIRE) {
        main.select();
      }
    });
  }
};
