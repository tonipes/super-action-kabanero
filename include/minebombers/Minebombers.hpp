#pragma once

#include <Box2D/Box2D.h>
#include "game/Game.hpp"
#include "physics/ContactListener.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/ui/GameTypeSelectedEvent.hpp"

class Minebombers : public Game {
public:
  Minebombers() : _physWorld(b2Vec2(0.0f,0.0f)), _phys_elapsed(0.0), _phys_step(0.033) {}

  auto init() -> void override;
  auto update(double delta) -> void override {
    Game::update(delta);
    _physWorld.Step(_phys_step, 8, 3);
  }
  auto startGame(GameType gameType) -> void;
private:
  b2World _physWorld;
  ContactListener _contactListener;
  double _phys_elapsed;
  const double _phys_step;
};
