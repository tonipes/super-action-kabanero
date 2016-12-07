#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class DamageAreaBehavior : public Behavior<Transform3D> {
public:
  DamageAreaBehavior(Node<Transform3D>* node, float damageTime) : _damageTime(damageTime){
    node->addEventReactor([&](CollisionEvent event) {

    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if(_alive > _damageTime) {
      Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(node.path())));
    }
    _alive += delta;
  }

private:
  float _alive = 0.0f;
  float _damageTime;
};
