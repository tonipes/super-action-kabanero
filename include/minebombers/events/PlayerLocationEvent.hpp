#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>


class PlayerLocationEvent : public Event {
public:
  PlayerLocationEvent(glm::vec2& vec) : x(vec.x), y(vec.y) { }
  auto getV() -> glm::vec2 { return glm::vec2(x,y); }
private:
  float x, y;
};
