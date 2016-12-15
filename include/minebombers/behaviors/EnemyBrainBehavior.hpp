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

class EnemyBrainBehavior : public EnemyBehavior {
public:
  EnemyBrainBehavior(Node* node, float difficulty) : EnemyBehavior(node, difficulty) {
    moveDirection.x = 2.0f * difficulty;
    _health = 20.0f + 50.0f * difficulty;
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {

    if (_collided) {
      const auto& physAttachment = node->get<PhysicsAttachment>();
      physAttachment.foreach([&](auto phys) {
        phys.setPosition(phys.position().x + -0.01f * moveDirection.x, phys.position().y + -0.01f * moveDirection.y);
      });

      if (moveDirection.x != 0) {

        moveDirection.y -= moveDirection.x;
        moveDirection.x = 0;
      } else {
        moveDirection.x = moveDirection.y;
        moveDirection.y = 0;
      }
      _collided = false;
    }

    updateCommon(delta, node);

  }
};
