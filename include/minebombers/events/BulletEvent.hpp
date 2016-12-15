#pragma once

#include "message/Event.hpp"
#include <string>
#include <glm/vec2.hpp>

enum BulletAction {
  CREATE_BULLET,
  DESTROY_BULLET,
};

class BulletEvent : public Event {
public:
  BulletEvent(
    BulletAction action,
    float x, float y,
    float vx, float vy,
    float speed, std::string target = ""
  ): _x(x), _y(y), _vx(vx), _vy(vy), _speed(speed), _action(action), _target(target) { }

  // auto getXY() -> glm::vec2 { return glm::vec2(_x,_y); }
  // auto getVelXY() -> glm::vec2 { return glm::vec2(_vx,_vy); }
  auto getX() -> float { return _x; }
  auto getY() -> float { return _y; }
  auto getVX() -> float { return _vx; }
  auto getVY() -> float { return _vy; }
  auto getSpeed() -> float { return _speed; }
  auto getAction() -> BulletAction { return _action; }
  auto getTarget() -> BulletAction { return _action; }

private:
  float _x, _y, _vx, _vy, _speed;
  std::string _target;
  BulletAction _action;
};
