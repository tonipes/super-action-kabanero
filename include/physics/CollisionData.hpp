#pragma once

#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include <string>
#include <memory>

class CollisionData {
public:
  CollisionData(std::string path, std::shared_ptr<CollisionMaterialAttachment> materialAttachment) :
    _path(path),
    _materialAttachment(materialAttachment) {}

  auto path() -> std::string {
    return _path;
  }
  
  auto collisionMaterialAttachment() -> std::shared_ptr<CollisionMaterialAttachment> {
    return _materialAttachment;
  }

private:
  std::string _path;
  std::shared_ptr<CollisionMaterialAttachment>  _materialAttachment;
};
