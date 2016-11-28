#pragma once

#include "message/Event.hpp"

enum GameInputAction {
  JUMP,
  FIRE,
  FIRE1,
  USE,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class GameInputEvent: public Event {
public:
  GameInputEvent(GameInputAction action, bool isActive): _action(action), _isPressed(isActive)  {}

  auto action() const -> GameInputAction {
    return _action;
  }

private:
  GameInputAction _action;
  bool _isPressed;
};
