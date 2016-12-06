#pragma once

#include "scene/NodeAttachment.hpp"
#include <string>
#include <iostream>
#include "minebombers/behaviors/TerrainBehaviour.hpp"
#include "minebombers/behaviors/ItemNodeBehaviour.hpp"

// This class is absolutely horrible. It should be refactored into subclasses,
// have proper setter functions, constructors and all kinds of things that
// it simply doesn't have. I'm sorry.
class CollisionMaterialAttachment : public NodeAttachment {
public:
  CollisionMaterialAttachment() {}

  // These are public because I don't want to write getters and setters.
  float collisionDamage = 0.0f;
  float bounciness = false;
  bool bulletRebound = false;
  bool staticMaterial = false;

  bool damageable = false;
  std::shared_ptr<TerrainBehaviour> terrainLink;

  bool hasItem = false;
  std::shared_ptr<ItemNodeBehaviour> itemLink;

  virtual auto takeDamage(float dmg) ->  void {
    if (damageable) terrainLink->takeDamage(dmg);
  }
};
