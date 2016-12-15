#pragma once

#include <string>
#include <memory>
#include "scene/NodeAttachment.hpp"
#include "graphics/Effect.hpp"

class EffectAttachment : public NodeAttachment {
public:
  EffectAttachment(std::shared_ptr<Effect> effect) : _effect(effect) {}

  auto effect() -> std::shared_ptr<Effect> {
    return _effect;
  }

  auto setEffect(std::shared_ptr<Effect> effect) -> void {
    _effect = std::move(effect);
  }


private:
  std::shared_ptr<Effect> _effect;
};
