#pragma once

#include "message/Event.hpp"

enum CollisionAction {
  TEST
};

class CollisionEvent: public Event {
public:
  CollisionEvent(CollisionAction action):
    _action(action) {}

  auto action() const -> CollisionAction {
    return _action;
  }

private:
  CollisionAction _action;
};
