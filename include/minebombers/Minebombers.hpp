#pragma once

#include <Box2D/Box2D.h>
#include "game/Game.hpp"
#include "physics/ContactListener.hpp"
#include "message/event/DestroyNodeEvent.hpp"

class Minebombers : public Game {
public:
  Minebombers() {}

  auto init() -> void override;
  // auto update(double delta) -> void override {
  //   Game::update(delta);
  //   // _physWorld.Step(_phys_step, 8, 3);
  // }
private:

};
