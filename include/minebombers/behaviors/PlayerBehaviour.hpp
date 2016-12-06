#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "minebombers/events/BulletEvent.hpp"
#include "minebombers/behaviors/BombBehaviour.hpp"
#include "minebombers/behaviors/EnemyOrbBehavior.hpp"
#include "message/event/CreateNodeEvent.hpp"
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
      } else if (action == FIRE) {
        throwBomb = isPressed;
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

    const auto& physAttachment = node.get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {
      const auto& pos = phys.position();
      // Services::logger()->debug(std::to_string(pos.x) + ", " + std::to_string(pos.y));
      // this->setLocalPosition(glm::vec3(pos.x, pos.y, this->localPosition().z));
      phys.setVelocity(moveDirection.x, moveDirection.y);
    });

    // auto pos = node.physics()->GetPosition();

    // node.setLocalPosition(glm::vec3(pos.x, pos.y, 2));
    auto pos = node.position().xy();
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

    if (throwBomb) {
      counter++;

      auto bombNode = std::make_shared<Node<Transform3D>>("bomb_"+std::to_string(counter));
      bombNode->setLocalPosition(glm::vec3(pos.x, pos.y, 5));

      auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/orb_of_destruction");
      auto material_att = std::make_shared<CollisionMaterialAttachment>();

      bombNode->addBehavior<BombBehaviour>(5.0f);
      // bombNode->addBehavior<EnemyOrbBehavior>();
      bombNode->addAttachment(sprite_att);
      bombNode->addAttachment(material_att);

      // b2BodyDef bodyDef;
      auto bodyDef = std::make_shared<b2BodyDef>();

      bodyDef->type = b2_dynamicBody;
      bodyDef->position.Set(pos.x, pos.y);
      bodyDef->allowSleep = false;
      bodyDef->fixedRotation = true;
      bodyDef->linearDamping = 0.5f;

      auto shape = std::make_shared<b2CircleShape>();
      shape->m_p.Set(0, 0);
      shape->m_radius = 0.2f;

      Services::messagePublisher()->sendMessage(Message("game", std::make_shared<CreateNodeEvent>(
        "world/bulletHandler", bodyDef, shape, bombNode
      )));

    }

    Services::messagePublisher()->sendMessage(Message("gameScene:world/fog", std::make_shared<PlayerLocationEvent>(pos2)));

    fireUp = false;
    fireRight = false;
    fireDown = false;
    fireLeft = false;
    throwBomb = false;
  }

private:
  int counter = 0;
  bool moveUp = false;
  bool moveRight = false;
  bool moveDown = false;
  bool moveLeft = false;

  bool fireUp = false;
  bool fireRight = false;
  bool fireDown = false;
  bool fireLeft = false;

  bool throwBomb = false;

};
