#pragma once

class MenuBehaviour : public Behavior<Transform3D> {
public:
  MenuBehaviour(Node<Transform3D>* node, MainMenu& main) {
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();

      if (action == UP) {
        moveUp = isPressed;
      } else if (action == DOWN) {
        moveDown = isPressed;
      }
    });
  }
};
