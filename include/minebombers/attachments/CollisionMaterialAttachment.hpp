#pragma once

#include "scene/NodeAttachment.hpp"
#include <string>
#include <iostream>
#include "minebombers/behaviors/TerrainBehaviour.hpp"

class CollisionMaterialAttachment : public NodeAttachment {
public:
  CollisionMaterialAttachment() {}

  // These are public because I don't want to write getters and setters.
  float collisionDamage = 0.0f;
  float bounciness = false; // Not implemented
  bool bulletRebound = false; // Not implemented

  bool damageable = false;
  std::shared_ptr<TerrainBehaviour> terrainLink;

  virtual auto takeDamage(float dmg) ->  void {
    if (damageable) terrainLink->takeDamage(dmg);
  }
};
