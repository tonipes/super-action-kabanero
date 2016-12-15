#pragma once

#include "scene/3D/Transform3D.hpp"
#include "scene/Node.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "minebombers/behaviors/PlayerBehavior.hpp"

class Player {
public:
  static auto create(std::string playerId, b2World& world) -> std::shared_ptr<Node>;
private:
  static auto createPhysCircle(b2World& world) -> b2Body*;

};
