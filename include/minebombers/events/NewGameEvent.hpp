#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

/**
 * An event used by the menu to tell the app it should start a new game.
 * Contains parameters for the number of players, seed that should be used in
 * level generation and game difficulty.
 */
class NewGameEvent : public Event {
public:
  NewGameEvent(int seed, int numPlayers, float difficulty = 0.5f) : difficulty(difficulty), seed(seed), numPlayers(numPlayers) { }

  int seed;
  int numPlayers;
  float difficulty;
};
