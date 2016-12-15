#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

class RespawnEvent : public Event {
public:
  RespawnEvent(int playerId, int livesLeft):
    _playerId(playerId), _livesLeft(livesLeft) {}

  auto getPlayerId() const -> int { return _playerId; }
  auto getLives() const -> int { return _livesLeft; }

private:
  int _playerId;
  int _livesLeft;

};
