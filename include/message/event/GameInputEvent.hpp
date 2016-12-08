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
  RIGHT,
  FIRE_UP,
  FIRE_DOWN,
  FIRE_LEFT,
  FIRE_RIGHT,
  NUM_1,
  NUM_2,
  NUM_3,
  NUM_4,
};

class GameInputEvent: public Event {
public:
  GameInputEvent(GameInputAction action, bool isActive): _action(action), _isPressed(isActive)  {}

  auto action() const -> GameInputAction {
    return _action;
  }

  auto isPressed() const -> bool {
    return _isPressed;
  }

private:
  GameInputAction _action;
  bool _isPressed;
};
