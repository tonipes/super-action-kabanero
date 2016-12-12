#pragma once

#include <string>
#include <memory>
#include "scene/NodeAttachment.hpp"
#include "graphics/Effect.hpp"

class HudAttachment : public NodeAttachment {
public:
  HudAttachment(std::shared_ptr<Effect> effect) : _effect(effect) {}

  auto effect() const -> std::shared_ptr<Effect> {
    return _effect;
  }

  auto setEffect(std::shared_ptr<Effect> effect) -> void {
    _effect = std::move(effect);
  }

private:
  std::shared_ptr<Effect> _effect;
};
