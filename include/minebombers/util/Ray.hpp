#pragma once

#include "minebombers/level/Tile.hpp"

class Ray {
public:
  Ray() {}

  auto registerTile(Tile& tile) -> void {
    _tiles += tile;
  }
  auto containsTile(Tile& tile) -> bool {
    return _tiles.exists([&tile](Tile t) { return t.getX() == tile.getX() && t.getY() == tile.getY(); });
  }
  auto getTiles() -> KBVector<Tile> {
    return _tiles;
  }

private:
  KBVector<Tile> _tiles;
};
