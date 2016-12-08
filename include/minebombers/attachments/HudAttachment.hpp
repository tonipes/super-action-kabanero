#pragma once

#include <string>
#include <memory>
#include "scene/NodeAttachment.hpp"
#include "graphics/Effect.hpp"
#include "minebombers/hud/HudEffect.hpp"

class HudAttachment : public NodeAttachment {
public:
  HudAttachment(std::shared_ptr<HudEffect> hud) : _hud(hud){}

  auto hud() -> std::shared_ptr<HudEffect> {
    return _hud;
  }

private:
  std::shared_ptr<HudEffect> _hud;
};
