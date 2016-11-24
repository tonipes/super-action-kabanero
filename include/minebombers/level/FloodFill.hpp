#pragma once

#include "minebombers/level/Cell.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "minebombers/level/TileMap.hpp"

class FloodFill {
public:
  FloodFill(KBVector<KBVector<Cell>>& map) : _map(map) {}

  auto calculateOpen() -> TileMap;
private:
  KBVector<KBVector<Cell>> _map;
  KBMap<int, int> _amounts;
  uint _groupCounter;
  auto flood(KBVector<Cell>& open, TileMap& mod) -> void;
};
