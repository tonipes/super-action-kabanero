#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/behaviors/EnemyBehavior.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class EnemyKnightBehavior : public EnemyBehavior {
public:
  EnemyKnightBehavior(Node<Transform3D>* node, float difficulty) : EnemyBehavior(node, difficulty) {
    moveDirection.x = 2.0f * difficulty;
    _health = 75.0f + 75.0f * difficulty;

    _shootInterval = 2.0f / difficulty;

  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if(_collided) {
      const auto& physAttachment = node.get<PhysicsAttachment>();
      physAttachment.foreach([&](auto phys) {
        phys.setPosition(phys.position().x + -0.01f * moveDirection.x, phys.position().y + -0.01f * moveDirection.y);
      });

      moveDirection.x = -moveDirection.x;
      moveDirection.y = -moveDirection.y;

      _collided = false;
    }
    auto pos = node.position().xy();

    auto gun_att = node.get<GunAttachment>();

    if(_shootTimer >= _shootInterval && gun_att.isDefined()){
      auto gunParams = gun_att.get().parameters();

      std::shared_ptr<Node<Transform3D>> bulletNode;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(bulletNode, bodyDef, fixtureDef) = NodeFactory::createBullet(gunParams);

      bodyDef->position.Set(
        pos.x + moveDirection.x,
        pos.y + moveDirection.y
      );
      bodyDef->linearVelocity.Set(
        moveDirection.x * gunParams->bulletSpeed,
        moveDirection.y * gunParams->bulletSpeed
      );

      Services::messagePublisher()->sendMessage(Message("gameScene", std::make_shared<CreateNodeEvent>(
        "world/bullets", bulletNode, bodyDef, fixtureDef
      )));

      _shootTimer = 0.0f;
    }

    _shootTimer += delta;
    updateCommon(delta, node);
  }

private:
  float _shootTimer = 0.0f;
  float _shootInterval = 0.0f;
};
