#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

class NewGameEvent : public Event {
public:
  NewGameEvent(int seed, int numPlayers, float difficulty = 0.5f) : difficulty(difficulty), seed(seed), numPlayers(numPlayers) { }

  int seed;
  int numPlayers;
  float difficulty;
};
