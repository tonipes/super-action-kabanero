#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "minebombers/hud/HudEffect.hpp"
// #include "minebombers/data/HudParameters.hpp"

#include "scene/3D/Transform3D.hpp"
#include "scene/attachment/EffectAttachment.hpp"
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
      _x = event.getX();
      _y = event.getY();
      _hp = event.getHP();
      _name = event.getName();
      needUpdate = true;
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {

    if(needUpdate) {
      const auto& effectAttachment = node.get<EffectAttachment>();
      node.addAttachment(std::make_shared<EffectAttachment>( std::make_shared<HudEffect>(_hp, _name) ));
      node.setLocalPosition(glm::vec3(_x, _y, 100.0f));

      needUpdate = false;
    }
  }

private:
  bool needUpdate = false;

  float _x, _y;
  int _hp;
  std::string _name;
};
