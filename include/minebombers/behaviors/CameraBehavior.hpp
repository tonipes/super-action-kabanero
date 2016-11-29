#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class CameraBehavior : public Behavior<Transform3D> {
public:
  CameraBehavior(Node<Transform3D>* node, float camSpeed) : _camSpeed(camSpeed) {
    node->addEventReactor([&](PlayerLocationEvent event) {
      _target = event.getV();
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    auto moveDirection = _target - node.position().xy();

    auto pos = node.position();
    node.setLocalPosition(pos + glm::vec3(moveDirection, 0) * _camSpeed);
  }

private:
  glm::vec2 _target;
  float _camSpeed;
};
