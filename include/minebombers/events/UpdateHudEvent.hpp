#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

class UpdateHudEvent : public Event {
public:
  UpdateHudEvent(
    float x, float y,
    float hp
  ) : _x(x), _y(y), _hp(hp) { }

  auto getX() -> float { return _x; }
  auto getY() -> float { return _y; }
  auto getHP() -> float { return _hp; }

private:
  float _x, _y, _hp;
};
