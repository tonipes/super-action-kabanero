#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "minebombers/util/NodeFactory.hpp"
#include "message/event/AudioClipEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class BombBehaviour : public Behavior<Transform3D> {
public:
  BombBehaviour(Node<Transform3D>* node, float fuseLength) :_fuseLength(fuseLength){
    node->addEventReactor([&](CollisionEvent event) {

    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if(!exploded){
      if(_fuseTime > _fuseLength){
        auto pos = node.position().xy();

        std::shared_ptr<Node<Transform3D>> damageNode;
        std::shared_ptr<b2BodyDef> bodyDef;
        std::shared_ptr<b2FixtureDef> fixtureDef;

        std::tie(damageNode, bodyDef, fixtureDef) = NodeFactory::createDamageCircle(3.0f, 1000.0f, 20.0f);
        bodyDef->position.Set(
          pos.x,
          pos.y
        );

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
};
