#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class BulletBehavior : public Behavior {
public:
  BulletBehavior(Node* node, float maxTime) :_maxTime(maxTime){
    node->addEventReactor([&, node](CollisionEvent event) {
      if(!event.collisionMaterialAttachment()->bulletRebound) {
        if (!destroyed) {
          destroyed = true;
          Services::messagePublisher()->sendMessage(Message("gameScene",std::make_shared<DestroyNodeEvent>(node->path())));
        }
      }
    });
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    // if(destroy) {
    //   Services::messagePublisher()->sendMessage(Message("gameScene",std::make_shared<DestroyNodeEvent>(node.path())));
    //   destroyed = false;
    // }
  }

private:
  bool destroyed = false;
  float _maxTime;
};
