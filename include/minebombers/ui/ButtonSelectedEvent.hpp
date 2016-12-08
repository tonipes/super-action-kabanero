#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

class ButtonSelectedEvent : public Event {
public:
  ButtonSelectedEvent(int i, bool press) : ind(i), pressed(press) { }
  auto index() -> int { return ind;}
  auto isPress() -> bool { return pressed; }
private:
  int ind;
  bool pressed;
};
