#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"


#include <glm/vec2.hpp>

#include <iostream>

class EnemyOrbBehavior : public Behavior {
public:
  EnemyOrbBehavior(Node* node){
    moveDirection.x = 2.0f;

    node->addEventReactor([&](CollisionEvent event) {

      if(event.collisionMaterialAttachment()->staticMaterial){
        turn = true;
      }
    });
  }

  auto update(float delta, Node& node) -> void override {

    if(turn) {

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
  float speed = 2.0f;
  bool turn = false;
};
