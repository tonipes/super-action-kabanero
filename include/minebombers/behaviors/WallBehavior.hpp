#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"

class WallBehavior : public Behavior {
public:
  WallBehavior(Node* node) {
    node->setSleep(true);
    node->addEventReactor([&](CollisionEvent event) {
      // Services::logger()->debug("wall collided");

    });
  }
private:

};
