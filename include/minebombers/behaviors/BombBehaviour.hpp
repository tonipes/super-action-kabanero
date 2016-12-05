#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class BombBehaviour : public Behavior<Transform3D> {
public:
  BombBehaviour(Node<Transform3D>* node, float fuseLength) :_fuseLength(fuseLength){
    node->addEventReactor([&](CollisionEvent event) {

    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    auto pos = node.physics()->GetPosition();
    node.setLocalPosition(glm::vec3(pos.x, pos.y, 2));
    if(!exploded){
      if(_fuseTime > _fuseLength){
        Services::logger()->debug("BOOM!");
        Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(node.path())));
        exploded = true;
      }
      _fuseTime += delta;
    }
  }

private:
  float _fuseLength;
  float _fuseTime = 0;
  bool exploded = false;
  bool destroy = false;

  // b2Body *_physBody;
};
