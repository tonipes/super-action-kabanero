#pragma once

#include "minebombers/level/TileMap.hpp"
#include "service/Random.hpp"

/**
 * SpawnPointPlacer divides the given tilemap into chunks, and places a player
 * spawn point into the chunk if possible. Then it does the same for items and
 * artifacts. The amount of chunks is parametrisable, with the true amount
 * being the given number^2. (playerSectors = 4 divides the map into 4x4=16
 * chunks)
 */
class SpawnPointPlacer {
public:
  SpawnPointPlacer(std::shared_ptr<TileMap>& map, int playerSectors, int itemSectors, Random& rand) :
   _map(map), _playerSectors(playerSectors), _itemSectors(itemSectors), _rand(rand) {}

  auto run() -> void;
private:
  Random& _rand;
  int _playerSectors, _itemSectors;
  std::shared_ptr<TileMap>& _map;
};
