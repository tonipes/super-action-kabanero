#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "minebombers/hud/HudEffect.hpp"

#include "scene/3D/Transform3D.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/events/UpdateHudEvent.hpp"
#include "minebombers/attachments/HudAttachment.hpp"
#include <glm/vec2.hpp>

#include <iostream>

class HudBehavior : public Behavior {
public:
  HudBehavior(Node* node) {
    node->addEventReactor([&](UpdateHudEvent event) {
      _text = event.getText();
      needUpdate = true;
    });
  }

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    if (needUpdate) {
      node->addAttachment(std::make_shared<HudAttachment>( std::make_shared<HudEffect>(_text) ));
      needUpdate = false;
    }
  }

private:
  bool needUpdate = false;
  std::string _text = "";
};
