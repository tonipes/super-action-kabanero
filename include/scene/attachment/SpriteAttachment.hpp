#pragma once

#include "scene/NodeAttachment.hpp"
#include <string>

/**
 * Sprite Attachment.
 * Holds information about sprite that is attached to the node.
 */
class SpriteAttachment : public NodeAttachment {
public:
  SpriteAttachment(std::string spriteId) : _spriteId(spriteId) {}

  auto spriteId() const -> std::string {
    return _spriteId;
  }

  auto setSpriteId(std::string id) -> void {
    _spriteId = id;
  }
private:
  std::string _spriteId;
};
