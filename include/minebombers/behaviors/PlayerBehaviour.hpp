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
#include "random/StdLibRandom.hpp"
#include "minebombers/attachments/GunAttachment.hpp"
#include "minebombers/util/NodeFactory.hpp"

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
    node->addEventReactor([&](CollisionEvent event) {
      if(event.collisionMaterialAttachment()->collisionDamage > 0.0f){
        takeDamage = true;
      }
      else if(event.collisionMaterialAttachment()->gunItem.isDefined()) {
        Services::logger()->debug("Player got new gun");
        _newGun = event.collisionMaterialAttachment()->gunItem;
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if (_newGun.isDefined()) {
      node.addAttachment(_newGun.get());
      _newGun = Option<std::shared_ptr<GunAttachment>>();
    }
    glm::vec2 moveDirection;
    if (moveUp) moveDirection.y += 1;
    if (moveDown) moveDirection.y -= 1;
    if (moveRight) moveDirection.x += 1;
    if (moveLeft) moveDirection.x -= 1;
    if (moveDirection.x != 0 && moveDirection.y != 0) {
      moveDirection.x /= 1.41421356237f;
      moveDirection.y /= 1.41421356237f;
    }

    moveDirection.x *= 4.5f;
    moveDirection.y *= 4.5f;

    const auto& physAttachment = node.get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {
      const auto& pos = phys.position();
      phys.setVelocity(moveDirection.x, moveDirection.y);
    });

    auto pos = node.position().xy();
    auto vel = physAttachment.get().velocity();

    Services::messagePublisher()->sendMessage(Message("gameScene:world/camera", std::make_shared<PlayerLocationEvent>(pos)));

    glm::vec2 fireDirection;
    _fireDelay -= delta;

    if (fireUp) fireDirection.y += 1;
    if (fireDown) fireDirection.y -= 1;
    if (fireRight) fireDirection.x += 1;
    if (fireLeft) fireDirection.x -= 1;
    if (fireDirection.x != 0 && fireDirection.y != 0) {
      fireDirection.x /= 1.41421356237f;
      fireDirection.y /= 1.41421356237f;
    }
    auto shoot = fireDirection.x != 0 || fireDirection.y != 0;

    if (shoot && _fireDelay <= 0) {
      auto gun = node.get<GunAttachment>().get();
      _fireDelay = 1.0f / gun.fireRate;
      auto random = StdLibRandom();
      for (auto i = 0; i < gun.bulletAmount; i++) {
        random.seed(_bulletsShot);
        auto xVar = random.nextFloat(), yVar = random.nextFloat(); // Bad variation, we shoud calculate angle and vary that instead
        auto spreadFactor = gun.accuracy;
        // fireDirection.x += (xVar * 2 * spreadFactor) - spreadFactor;
        // fireDirection.y += (yVar * 2 * spreadFactor) - spreadFactor;
        _bulletsShot++;

        std::shared_ptr<Node<Transform3D>> bulletNode;
        std::shared_ptr<b2BodyDef> bodyDef;
        std::shared_ptr<b2FixtureDef> fixtureDef;

        std::tie(bulletNode, bodyDef, fixtureDef) = NodeFactory::createBullet();
        bodyDef->position.Set(
          pos.x + fireDirection.x,
          pos.y + fireDirection.y
        );
        bodyDef->linearVelocity.Set(
          fireDirection.x * gun.bulletSpeed,
          fireDirection.y * gun.bulletSpeed
        );

        Services::messagePublisher()->sendMessage(Message("game", std::make_shared<CreateNodeEvent>(
          "world/bullets", bulletNode, bodyDef, fixtureDef
        )));
      }

      Services::messagePublisher()->sendMessage(
        Message(
          "audioPlayer:clip/gunshot.ogg",
          std::make_shared<AudioClipEvent>(CLIP_PLAY)
        )
      );
    }

    if (throwBomb) {
      std::shared_ptr<Node<Transform3D>> bombNode;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(bombNode, bodyDef, fixtureDef) = NodeFactory::createBomb();
      bodyDef->position.Set(pos.x, pos.y);

      Services::messagePublisher()->sendMessage(Message("game", std::make_shared<CreateNodeEvent>(
        "world/bullets", bombNode, bodyDef, fixtureDef
      )));
      Services::messagePublisher()->sendMessage(Message(
        "audioPlayer:clip/set_bomb.ogg",
        std::make_shared<AudioClipEvent>(CLIP_PLAY)
      ));
    }

    if(takeDamage) {
      Services::messagePublisher()->sendMessage(Message(
        "audioPlayer:clip/pain.ogg",
        std::make_shared<AudioClipEvent>(CLIP_PLAY)
      ));
      takeDamage = false;
    }

    Services::messagePublisher()->sendMessage(Message("gameScene:world/fog", std::make_shared<PlayerLocationEvent>(pos)));

    throwBomb = false;
  }

private:
  int counter = 0;
  bool takeDamage = false;
  bool moveUp = false;
  bool moveRight = false;
  bool moveDown = false;
  bool moveLeft = false;

  bool fireUp = false;
  bool fireRight = false;
  bool fireDown = false;
  bool fireLeft = false;

  int _bulletsShot = 0;

  float _fireDelay = 0;

  bool throwBomb = false;

  Option<std::shared_ptr<GunAttachment>> _newGun = Option<std::shared_ptr<GunAttachment>>();
};
