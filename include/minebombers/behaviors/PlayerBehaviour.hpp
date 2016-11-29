#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include <Box2D/Box2D.h>

#include <glm/vec2.hpp>

class PlayerBehaviour : public Behavior<Transform3D> {
public:
  PlayerBehaviour(Node<Transform3D>* node, b2Body *physBody) : _physBody(physBody) {
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
    if (moveDirection.x != 0 && moveDirection.y != 0) { // Focken geniuous
      moveDirection.x /= 1.41421356237f;
      moveDirection.y /= 1.41421356237f;
    }

    moveDirection.x *= 4.5f;
    moveDirection.y *= 4.5f;

    _physBody->SetLinearVelocity(b2Vec2(moveDirection.x, moveDirection.y));
    auto pos = _physBody->GetPosition();

    node.setLocalPosition(glm::vec3(pos.x, pos.y, 2));
    auto pos2 = node.position().xy();
    Services::messagePublisher()->sendMessage(Message("gameScene:camera", std::make_shared<PlayerLocationEvent>(pos2)));
  }

private:
  b2Body *_physBody;
  bool moveUp = false;
  bool moveRight = false;
  bool moveDown = false;
  bool moveLeft = false;
};
