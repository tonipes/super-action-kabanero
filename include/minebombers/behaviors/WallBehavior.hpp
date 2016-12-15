#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"

class WallBehavior : public Behavior<Transform3D> {
public:
  WallBehavior(Node<Transform3D>* node) {
    node->setSleep(true);
    node->addEventReactor([&](CollisionEvent event) {
      // Services::logger()->debug("wall collided");

    });
  }
private:

};
