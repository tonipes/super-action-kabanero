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
      if(!event.collisionMaterialAttachment()->bulletRebound){
        destroy = true;
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if(destroy) {
      Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(node.path())));
      destroy = false;
    }
  }

private:
  bool destroy = false;
  float _maxTime;
};
