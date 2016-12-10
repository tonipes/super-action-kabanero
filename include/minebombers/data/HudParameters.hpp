#pragma once

#include <string>

class HudParameters {
public:
  HudParameters(float x, float y, int hp, std::string name):
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
