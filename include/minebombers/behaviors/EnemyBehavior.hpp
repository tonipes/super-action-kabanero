#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class EnemyBehavior : public Behavior<Transform3D> {
public:
  EnemyBehavior(Node<Transform3D>* node, float difficulty) : _difficulty(difficulty){
    moveDirection.x = 0.0f;
    moveDirection.y = 0.0f;

    node->addEventReactor([&](CollisionEvent event) {
      if(event.collisionMaterialAttachment()->collisionDamage > 0.0f){
        _dmgToTake += event.collisionMaterialAttachment()->collisionDamage;
      }
      _collided = true;
    });

  }

  virtual auto update(float delta, Node<Transform3D>& node) -> void override {

  }

protected:
  auto updateCommon(float delta, Node<Transform3D>& node) -> void {
    const auto& physAttachment = node.get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {
      phys.setVelocity(moveDirection.x, moveDirection.y);
    });

    if (_dmgToTake > 0.0f) {
      _health -= _dmgToTake;
      _dmgToTake = 0;
      if (_health <= 0.0f) {
        Services::messagePublisher()->sendMessage(
          Message(
            "gameScene",
            std::make_shared<DestroyNodeEvent>(node.path())
          )
        );
      }
    }
  }

  float _dmgToTake = 0.0f;
  glm::vec2 moveDirection;
  float _difficulty;
  float _health = 0.0f;
  bool _collided = false;

};
