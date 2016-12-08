#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/events/UpdateHudEvent.hpp"
#include <glm/vec2.hpp>

#include <iostream>

class HudBehavior : public Behavior<Transform3D> {
public:
  HudBehavior(Node<Transform3D>* node) {
    node->addEventReactor([&](UpdateHudEvent event) {
      // Should have some kind of datacontainer
      x = event.getX();
      y = event.getY();
      hp = event.getHP();
      needUpdate = true;
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    const auto& hudAttachment = node.get<HudAttachment>();

    if(needUpdate) {
      hudAttachment.foreach([&](auto hud_att) {
        const auto& hud = hud_att.hud();
        hud->update(hp);
      });

      node.setLocalPosition(glm::vec3(x, y, 100.0f));

      needUpdate = false;
    }
  }

private:
  bool needUpdate = false;
  float x = 0.0f;
  float y = 0.0f;
  int hp = 0.0f;
};
