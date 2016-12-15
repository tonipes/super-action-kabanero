#pragma once

#include "minebombers/level/TileMap.hpp"
#include "service/Random.hpp"

/**
 * Randomly places constructed walls or windows to a TileMap. The algorithm
 * places walls into the 'main' area of a level, trying to keep it connected.
 * Rarely produces cases where the area is divided into multiple parts. This
 * won't matter though, as the game features fully destructible terrain
 */
class ConstructedWallPlacer {
public:
  ConstructedWallPlacer(std::shared_ptr<TileMap>& map, int numWalls, int minWallLen, int maxWallLen, Random& rand):
   _map(map), _numWalls(numWalls), _minWallLen(minWallLen), _maxWallLen(maxWallLen), _rand(rand) {}

  auto run() -> void;
private:
  auto process(KBVector<Tile>& processed, Tile& tile, int xDir, int yDir, int invXDir, int invYDir) -> KBVector<Tile>&;
  Random& _rand;
  int _numWalls, _minWallLen, _maxWallLen;
  std::shared_ptr<TileMap>& _map;
};
