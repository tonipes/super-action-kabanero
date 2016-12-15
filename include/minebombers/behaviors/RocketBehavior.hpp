#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/AudioClipEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

#include "minebombers/util/NodeFactory.hpp"

class RocketBehavior : public Behavior {
public:
  RocketBehavior(Node* node, float radius, float damage) :
    _radius(radius),
    _damage(damage)
  {
    node->addEventReactor([&](CollisionEvent event) {
      explode = true;
    });
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    if (explode && !alreadyExploded) {
      const auto pos = node->position();

      std::shared_ptr<Node> damageNode;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(damageNode, bodyDef, fixtureDef) = NodeFactory::createDamageCircle(_radius, _damage, 20.0f);
      bodyDef->position.Set(pos.x, pos.y);

      Services::messagePublisher()->sendMessage(Message("gameScene", std::make_shared<CreateNodeEvent>(
        "world/bullets", damageNode, bodyDef, fixtureDef
      )));

      Services::messagePublisher()->sendMessage(
        Message(
          "audioPlayer:clip/granade.ogg",
          std::make_shared<AudioClipEvent>(CLIP_PLAY)
        )
      );

      Services::messagePublisher()->sendMessage(Message("gameScene",std::make_shared<DestroyNodeEvent>(node->path())));

      explode = false;
      alreadyExploded = true;
    }
  }

private:
  bool explode = false;
  bool alreadyExploded = false;

  float _radius;
  float _damage;
};
