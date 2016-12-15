#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/Event.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"


// Keeps node rotation to face same direction as the body is moving
class BulletOrientationBehavior : public Behavior {
public:
  BulletOrientationBehavior(Node* node) {
    // node->addEventReactor([&](CollisionEvent event) {
    //   updateOrientation = true;
    // });
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    if(updateOrientation) {
      auto physicsAttachment = node->get<PhysicsAttachment>();

      if(physicsAttachment.isDefined()){
        auto vel = physicsAttachment.get().velocity();
        auto rot = glm::atan(vel.x, vel.y);
        node->setLocalRotation(glm::angleAxis(rot, glm::vec3(0.0f, 0.0f, 1.0f)));
      }

      updateOrientation = false;
    }
  }

private:
  bool updateOrientation = true;
};
