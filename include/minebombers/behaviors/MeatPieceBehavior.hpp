#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

#include "minebombers/util/NodeFactory.hpp"

class MeatPieceBehavior : public Behavior {
public:
  MeatPieceBehavior(Node* node, float lifetime, float trailInterval = 0.3f) :
    _lifetime(lifetime), _trailInterval(trailInterval) {
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    auto pos = node->position();

    if(_trailTimer >= _trailInterval) {
      std::shared_ptr<Node> bloodNode;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(bloodNode, bodyDef, fixtureDef) = NodeFactory::createBloodstain();

      bodyDef->position.Set(
        pos.x,
        pos.y
      );

      Services::messagePublisher()->sendMessage(Message("gameScene", std::make_shared<CreateNodeEvent>(
        "world/bullets", bloodNode, bodyDef, fixtureDef
      )));
      _trailTimer = 0.0f;
    }
    if(_timer > _lifetime && !alreadyDeleted) {
      Services::messagePublisher()->sendMessage(Message("gameScene",std::make_shared<DestroyNodeEvent>(node->path())));
      alreadyDeleted = true;
    }
    _timer += delta;
    _trailTimer += delta;
  }

private:
  bool alreadyDeleted = false;
  float _timer = 0.0f;
  float _trailTimer = 0.0f;
  float _trailInterval;
  float _lifetime;
};
