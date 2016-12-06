#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class BulletBehavior : public Behavior<Transform3D> {
public:
  BulletBehavior(Node<Transform3D>* node, float maxTime) :_maxTime(maxTime){
    node->addEventReactor([&](CollisionEvent event) {
      // Services::logger()->debug("Bullet collided with [" + event.path() + "]");
      // Services::logger()->debug("BulletRebound [" + std::to_string(event.collisionMaterialAttachment()->bulletRebound) + "]");
      if(!event.collisionMaterialAttachment()->bulletRebound){
        destroy = true;
      }

      event.collisionMaterialAttachment()->takeDamage(10.0f);

    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    // Services::logger()->debug("Bullet update");
    // auto pos = node.physics()->GetPosition();
    // node.setLocalPosition(glm::vec3(pos.x, pos.y, 2));

    if(destroy) {
      // Services::logger()->debug("destroy");
      Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(node.path())));
      destroy = false;
    }
  }

private:
  bool destroy = false;
  float _maxTime;
  // b2Body *_physBody;
};
