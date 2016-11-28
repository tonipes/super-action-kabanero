#pragma once

#include "scene/NodeAttachment.hpp"
#include <string>

class SpriteAttachment : public NodeAttachment {
public:
  SpriteAttachment(std::string spriteId) : _spriteId(spriteId) {}

  auto spriteId() -> std::string {
    return _spriteId;
  }
private:
  std::string _spriteId;
};
