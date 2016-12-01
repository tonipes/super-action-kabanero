#pragma once

#include "game/Game.hpp"

class App {
public:
  App(std::shared_ptr<Game> game);

  auto init() -> void;

  auto run() -> void;
private:
  std::shared_ptr<Game> _game;
  double _update_interval;
  double _draw_interval;
  int _window_w;
  int _window_h;
  int _tilesize;
  std::string _window_name;
  std::string _audioFolderPath;
};
