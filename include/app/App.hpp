#pragma once

#include "game/Game.hpp"

class App {
public:
  App(Game game);

  auto init() -> void;

  auto run() -> void;
private:
  Game _game;
  int _update_interval;
  int _draw_interval;
  int _window_w;
  int _window_h;
  std::string _window_name;
  std::string _audioFolderPath;
};
