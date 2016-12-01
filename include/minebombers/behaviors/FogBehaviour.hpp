#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class FogBehaviour : public Behavior<Transform3D> {
public:
  FogBehaviour(Node<Transform3D>* node, std::shared_ptr<FogMap> fog) : _fog(fog), x(-1), y(-1) {
    node->addEventReactor([&](PlayerLocationEvent event) {
      x = round(event.getX());
      y = round(event.getY());
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if (x == -1 || y == -1) return;
    if (!_fog->isVisited(x, y)) {
      (*_fog)[x][y]->setRenderOn(false);
      _fog->markVisited(x,y);
    }
  }

private:
  int x, y;
  std::shared_ptr<FogMap> _fog;
};
