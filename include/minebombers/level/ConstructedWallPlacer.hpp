#pragma once

#include "minebombers/level/TileMap.hpp"
#include "service/Random.hpp"

class ConstructedWallPlacer {
public:
  ConstructedWallPlacer(TileMap& map, int numWalls, int minWallLen, int maxWallLen, Random& rand):
   _map(map), _numWalls(numWalls), _minWallLen(minWallLen), _maxWallLen(maxWallLen), _rand(rand) {}

  auto run() -> void;
private:
  auto process(KBVector<Tile>& processed, Tile& tile, int xDir, int yDir, int invXDir, int invYDir) -> KBVector<Tile>&;
  Random& _rand;
  int _numWalls, _minWallLen, _maxWallLen;
  TileMap& _map;
};
