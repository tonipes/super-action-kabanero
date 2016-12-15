#pragma once

#include <iostream>
// #include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/level/TileMap.hpp"
#include "scene/3D/Transform3D.hpp"
#include "game/Behavior.hpp"
#include "service/Services.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CollisionEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "random/StdLibRandom.hpp"
#include "util/StringUtil.hpp"
#include "minebombers/attachments/GunAttachment.hpp"

class TerrainBehavior : public Behavior {
public:
  TerrainBehavior(Node* node, float health, std::shared_ptr<TileMap> map);

  auto update(float delta, std::shared_ptr<Node> node) -> void override;

private:
  float _health = 0.0f;
  float _dmgToTake = 0.0f;
  std::shared_ptr<TileMap> _map;
};
