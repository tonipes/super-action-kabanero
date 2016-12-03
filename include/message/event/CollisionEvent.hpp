#pragma once

#include "message/Event.hpp"

enum CollisionAction {
  BEGIN,
  END
};

class CollisionEvent: public Event {
public:
  CollisionEvent(CollisionAction action, std::string otherPath):
    _action(action), _otherPath(otherPath) {}

  auto action() const -> CollisionAction {
    return _action;
  }
  
  auto path() const -> std::string {
    return _otherPath;
  }

private:
  std::string _otherPath;
  CollisionAction _action;
};
