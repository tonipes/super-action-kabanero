#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/attachments/GunAttachment.hpp"

#include <glm/vec2.hpp>

#include <iostream>
#include <memory>

class ItemNodeBehaviour : public Behavior {
public:
  ItemNodeBehaviour(Node* node) {
    node->addEventReactor([&, node](CollisionEvent event) {
      if(event.collisionMaterialAttachment()->isPlayer){
        destroy = true;
        node->wakeUp();
      }
    });
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    if(destroy) {
      Services::logger()->debug("destroy itemnode");
      Services::messagePublisher()->sendMessage(Message("gameScene",std::make_shared<DestroyNodeEvent>(node->path())));
      destroy = false;
    }
  }
private:
  bool destroy = false;
};
