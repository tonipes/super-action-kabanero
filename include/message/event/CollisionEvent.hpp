#pragma once

#include "message/Event.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include <glm/vec2.hpp>

enum CollisionAction {
  BEGIN,
  END
};

class CollisionEvent: public Event {
public:
  CollisionEvent(
    CollisionAction action,
    std::string otherPath,
    std::shared_ptr<CollisionMaterialAttachment> materialAttachment,
    bool sensor,
    glm::vec2 collisionVec):
  _action(action),
  _otherPath(otherPath),
  _materialAttachment(materialAttachment),
  _sensor(sensor),
  _collisionVec(collisionVec) {}

  auto action() const -> CollisionAction {
    return _action;
  }

  auto path() const -> std::string {
    return _otherPath;
  }

  auto sensor() const -> bool {
    return _sensor;
  }
  auto vec() const -> glm::vec2 {
    return _collisionVec;
  }

  auto collisionMaterialAttachment() const -> std::shared_ptr<CollisionMaterialAttachment>{
    return _materialAttachment;
  }

private:
  glm::vec2 _collisionVec;
  std::string _otherPath;
  bool _sensor;
  CollisionAction _action;
  std::shared_ptr<CollisionMaterialAttachment> _materialAttachment;

};
