#pragma once

#include "scene/NodeAttachment.hpp"
#include <string>

class CollisionMaterialAttachment : public NodeAttachment {
public:
  CollisionMaterialAttachment() {}

  // These are public because I don't want to write getters and setters.
  float collisionDamage = 0.0f;
  float bounciness = false;
  bool bulletRebound = false;
  bool staticMaterial = false; 
};
