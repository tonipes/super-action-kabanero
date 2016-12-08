#pragma once

#include "minebombers/ui/MainMenuData.hpp"


class MenuBehaviour : public Behavior<Transform3D> {
public:
  MenuBehaviour(Node<Transform3D>* node, MainMenuData& data) {
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();
      if (action == UP) {
        data.changeSelection(-1);
      } else if (action == DOWN) {
        data.changeSelection(1);
      } else if (action == FIRE) {
        data.select();
      }
    });
  }
};
