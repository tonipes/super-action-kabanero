#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/GameInputEvent.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class MainMenuBehavior : public Behavior<Transform3D> {
public:
  MainMenuBehavior(Node<Transform3D>* node) {
    node->addEventReactor([&, node](GameInputEvent event) {
      Services::logger()->debug("Menu input event");
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {

  }

private:
  bool destroyed = false;
  float _maxTime;
};
