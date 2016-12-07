#pragma once

#include "scene/NodeAttachment.hpp"
#include "collection/Option.hpp"
#include "minebombers/attachments/GunAttachment.hpp"
#include "minebombers/data/GunParameters.hpp"
#include <string>
#include <iostream>

class CollisionMaterialAttachment : public NodeAttachment {
public:
  CollisionMaterialAttachment() {}

  float collisionDamage = 0.0f;
  float force = 0.0f;
  float bounciness = false;
  bool bulletRebound = false;
  bool staticMaterial = false;
  bool isPlayer = false;

  Option<std::shared_ptr<GunParameters>> gunParameters = Option<std::shared_ptr<GunParameters>>();

};
