#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "minebombers/events/BulletEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "message/event/AudioClipEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include <Box2D/Box2D.h>

#include <glm/vec2.hpp>

class PlayerBehaviour : public Behavior<Transform3D> {
public:
  PlayerBehaviour(Node<Transform3D>* node) {
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
      } else if (action == FIRE_RIGHT) {
        fireRight = isPressed;
      } else if (action == FIRE_DOWN) {
        fireDown = isPressed;
      } else if (action == FIRE_LEFT) {
        fireLeft = isPressed;
      } else if (action == FIRE_UP) {
        fireUp = isPressed;
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

    node.physics()->SetLinearVelocity(b2Vec2(moveDirection.x, moveDirection.y));
    auto pos = node.physics()->GetPosition();

    node.setLocalPosition(glm::vec3(pos.x, pos.y, 2));
    auto pos2 = node.position().xy();

    Services::messagePublisher()->sendMessage(Message("gameScene:world/camera", std::make_shared<PlayerLocationEvent>(pos2)));

    glm::vec2 fireDirection;

    if (fireUp) fireDirection.y += 1;
    if (fireDown) fireDirection.y -= 1;
    if (fireRight) fireDirection.x += 1;
    if (fireLeft) fireDirection.x -= 1;
    if (fireDirection.x != 0 && fireDirection.y != 0) {
      fireDirection.x /= 1.41421356237f;
      fireDirection.y /= 1.41421356237f;
    }
    auto shoot = fireDirection.x != 0 || fireDirection.y != 0;

    if (shoot) {
      Services::messagePublisher()->sendMessage(Message("gameScene:world/bulletHandler",
        std::make_shared<BulletEvent>(CREATE_BULLET, pos.x, pos.y, fireDirection.x, fireDirection.y, 20.0f)));

      Services::messagePublisher()->sendMessage(
        Message(
          "audioPlayer:clip/gunshot.ogg",
          std::make_shared<AudioClipEvent>(CLIP_PLAY)
        )
      );
    }

    Services::messagePublisher()->sendMessage(Message("gameScene:world/fog", std::make_shared<PlayerLocationEvent>(pos2)));

    fireUp = false;
    fireRight = false;
    fireDown = false;
    fireLeft = false;
  }

private:
  bool moveUp = false;
  bool moveRight = false;
  bool moveDown = false;
  bool moveLeft = false;

  bool fireUp = false;
  bool fireRight = false;
  bool fireDown = false;
  bool fireLeft = false;

};
