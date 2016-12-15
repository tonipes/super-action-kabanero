#pragma once

#include "minebombers/level/Cell.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "minebombers/level/TileMap.hpp"

/**
 * Outputs a TileMap from 2d array of cells. This tool finds the largest
 * connected area, and divides other areas into side areas using the flood fill
 * algorithm. Outputs a fully usable TileMap, which can be further tweaked.
 */
class FloodFill {
public:
  FloodFill(KBVector<KBVector<Cell>>& map) : _map(map) {}

  auto calculateOpen() -> std::shared_ptr<TileMap>;
private:
  KBVector<KBVector<Cell>> _map;
  KBMap<int, int> _amounts;
  uint _groupCounter;
  auto flood(KBVector<Cell>& open, std::shared_ptr<TileMap>& mod) -> void;
};
