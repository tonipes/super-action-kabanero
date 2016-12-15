#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

#include "minebombers/util/NodeFactory.hpp"

class BloodstainBehavior : public Behavior<Transform3D> {
public:
  BloodstainBehavior(Node<Transform3D>* node, float lifetime) :
    _lifetime(lifetime) {

  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if(_timer > _lifetime && !alreadyDeleted) {
      Services::messagePublisher()->sendMessage(Message("gameScene",std::make_shared<DestroyNodeEvent>(node.path())));
      alreadyDeleted = true;
    }
    _timer += delta;
  }

private:
  bool alreadyDeleted = false;
  float _timer = 0.0f;
  float _lifetime;
};
