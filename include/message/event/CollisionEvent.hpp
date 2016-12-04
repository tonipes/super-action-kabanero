#pragma once

#include "message/Event.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"

enum CollisionAction {
  BEGIN,
  END
};

class CollisionEvent: public Event {
public:
  CollisionEvent(
    CollisionAction action,
    std::string otherPath,
    std::shared_ptr<CollisionMaterialAttachment> materialAttachment):
  _action(action),
  _otherPath(otherPath),
  _materialAttachment(materialAttachment) {}

  auto action() const -> CollisionAction {
    return _action;
  }

  auto path() const -> std::string {
    return _otherPath;
  }
  auto collisionMaterialAttachment() const -> std::shared_ptr<CollisionMaterialAttachment>{
    return _materialAttachment;
  }

private:
  std::string _otherPath;
  CollisionAction _action;
  std::shared_ptr<CollisionMaterialAttachment> _materialAttachment;

};
