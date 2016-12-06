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
        // changeGun = true;
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
    auto pos2 = node.position().xy();

    Services::messagePublisher()->sendMessage(Message("gameScene:world/camera", std::make_shared<PlayerLocationEvent>(pos2)));

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
        Services::messagePublisher()->sendMessage(Message("gameScene:world/bulletHandler",
          std::make_shared<BulletEvent>(CREATE_BULLET, pos.x, pos.y, fireDirection.x, fireDirection.y, gun.bulletSpeed)));
      }
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
      bombNode->addAttachment(sprite_att);
      bombNode->addAttachment(material_att);

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

    if(takeDamage) {
      Services::messagePublisher()->sendMessage(Message(
        "audioPlayer:clip/pain.ogg",
        std::make_shared<AudioClipEvent>(CLIP_PLAY)
      ));
      takeDamage=false;
    }

    Services::messagePublisher()->sendMessage(Message("gameScene:world/fog", std::make_shared<PlayerLocationEvent>(pos2)));

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

  bool changeGun = false;
  Option<std::shared_ptr<GunAttachment>> _newGun = Option<std::shared_ptr<GunAttachment>>();
  // std::shared_ptr<GunAttachment> _newGun;
};
