#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class EnemyBrainBehavior : public Behavior<Transform3D> {
public:
  EnemyBrainBehavior(Node<Transform3D>* node, float speed) : _speed(speed){
    moveDirection.x = _speed;

    node->addEventReactor([&](CollisionEvent event) {
      // if(event.collisionMaterialAttachment()->staticMaterial){
        turn = true;
      // }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    // Services::logger()->debug("enemy update " + std::to_string(moveDirection.x) + ", " + std::to_string(moveDirection.y));

    if(turn) {
      const auto& physAttachment = node.get<PhysicsAttachment>();
      physAttachment.foreach([&](auto phys) {
        phys.setPosition(phys.position().x + -0.01f * moveDirection.x, phys.position().y + -0.01f * moveDirection.y);
      });

      if(moveDirection.x != 0){

        moveDirection.y -= moveDirection.x;
        moveDirection.x = 0;
      }
      else {
        moveDirection.x = moveDirection.y;
        moveDirection.y = 0;
      }
      turn = false;
    }

    const auto& physAttachment = node.get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {

      phys.setVelocity(moveDirection.x, moveDirection.y);
    });
  }

private:
  glm::vec2 moveDirection;
  float _speed;
  bool turn = false;
};
