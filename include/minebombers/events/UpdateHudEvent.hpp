#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>
#include "minebombers/data/HudParameters.hpp"

class UpdateHudEvent : public Event {
public:
  UpdateHudEvent(float x, float y, int hp, std::string name):
    _x(x), _y(y), _hp(hp), _name(name) {}

  auto getX() const -> float { return _x; }
  auto getY() const -> float { return _y; }
  auto getHP() const -> int { return _hp; }
  auto getName() const -> std::string { return _name; }

private:
  float _x, _y;
  int _hp;
  std::string _name;

};
