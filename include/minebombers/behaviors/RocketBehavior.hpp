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

class RocketBehavior : public Behavior<Transform3D> {
public:
  RocketBehavior(Node<Transform3D>* node, float radius, float damage) :
    _radius(radius),
    _damage(damage)
  {
    node->addEventReactor([&](CollisionEvent event) {
      explode = true;
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if(explode) {
      auto pos = node.position().xy();

      std::shared_ptr<Node<Transform3D>> damageNode;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(damageNode, bodyDef, fixtureDef) = NodeFactory::createDamageCircle(_radius, _damage, 20.0f);
      bodyDef->position.Set(pos.x,pos.y);

      Services::messagePublisher()->sendMessage(Message("game", std::make_shared<CreateNodeEvent>(
        "world/bullets", damageNode, bodyDef, fixtureDef
      )));

      Services::messagePublisher()->sendMessage(
        Message(
          "audioPlayer:clip/granade.ogg",
          std::make_shared<AudioClipEvent>(CLIP_PLAY)
        )
      );

      Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(node.path())));

      explode = false;
    }
  }

private:
  bool explode = false;

  float _radius;
  float _damage;
};
