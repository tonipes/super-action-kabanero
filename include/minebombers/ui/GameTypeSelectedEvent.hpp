#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>
#include "minebombers/GameType.hpp"

class GameTypeSelectedEvent : public Event {
public:
  GameTypeSelectedEvent(GameType ty) : type(ty) { }
  auto getType() -> GameType { return type; }
private:
  GameType type;
};
