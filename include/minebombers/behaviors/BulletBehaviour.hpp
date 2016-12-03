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
  BulletBehavior(Node<Transform3D>* node, b2Body *physBody, float maxTime) : _physBody(physBody), _maxTime(maxTime){
    node->addEventReactor([&](CollisionEvent event) {
      destroy = true;
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    // Services::logger()->debug("Bullet update");
    auto pos = _physBody->GetPosition();
    node.setLocalPosition(glm::vec3(pos.x, pos.y, 2));

    if(destroy) {
      Services::messagePublisher()->sendMessage(Message("gameScene:world/bulletHandler",std::make_shared<DestroyNodeEvent>(node.path())));
    }
  }

private:
  bool destroy = false;
  float _maxTime;
  b2Body *_physBody;
};
