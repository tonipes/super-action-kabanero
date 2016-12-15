#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "minebombers/attachments/VisibilityAttachment.hpp"
#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class CameraBehavior : public Behavior {
public:
  CameraBehavior(Node* node, float camSpeed) : _camSpeed(camSpeed) {
    node->addEventReactor([&](PlayerLocationEvent event) {
      _target = event.getV();
    });
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    auto pos = node->position();

    auto moveDirection = _target - pos.xy();

    node->setLocalPosition(pos + glm::vec3(moveDirection, 0) * _camSpeed);

    const auto& visibilityAttachment = node->get<VisibilityAttachment>();
    if (visibilityAttachment.isDefined()) {
      visibilityAttachment.get().markVisited(round(_target.x), round(_target.y));
    }
  }

private:
  glm::vec2 _target;
  float _camSpeed;
};
