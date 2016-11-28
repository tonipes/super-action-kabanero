#pragma once

#include "minebombers/events/TestEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

#include <glm/vec2.hpp>

class CameraBehavior : public Behavior<Transform3D> {
public:
  CameraBehavior(Node<Transform3D>* node) {
    node->addEventReactor([&](TestEvent event) {
      auto action = event.action();
      if (action == A) {
        Services::logger()->debug("A");
      } else if (action == B) {
        Services::logger()->debug("B");
      }
    });
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();
      if (action == UP) {
        moveUp = isPressed;
      } else if (action == RIGHT) {
        moveRight = isPressed;
      } else if (action == DOWN) {
        moveDown = isPressed;
      } else if (action == LEFT) {
        moveLeft = isPressed;
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    glm::vec2 moveDirection;
    if (moveUp) moveDirection.y += 1;
    if (moveDown) moveDirection.y -= 1;
    if (moveRight) moveDirection.x += 1;
    if (moveLeft) moveDirection.x -= 1;

    auto pos = node.position();
    node.setLocalPosition(pos + glm::vec3(moveDirection, 0));
  }

private:
  bool moveUp = false;
  bool moveRight = false;
  bool moveDown = false;
  bool moveLeft = false;
};
