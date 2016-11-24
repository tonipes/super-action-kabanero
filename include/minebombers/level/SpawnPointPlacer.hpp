#pragma once

#include "minebombers/level/TileMap.hpp"
#include "service/Random.hpp"

class SpawnPointPlacer {
public:
  SpawnPointPlacer(TileMap& map, int playerSectors, int itemSectors, Random& rand) :
   _map(map), _playerSectors(playerSectors), _itemSectors(itemSectors), _rand(rand) {}

  auto run() -> void;
private:
  Random& _rand;
  int _playerSectors, _itemSectors;
  TileMap& _map;
};
