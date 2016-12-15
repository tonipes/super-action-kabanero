#include "minebombers/level/ConstructedWallPlacer.hpp"

auto ConstructedWallPlacer::run() -> void {
  int xDir, yDir, invXDir, invYDir;
  int tries = 0;
  const auto MAX_TRIES = 50;
  for (auto w = 0; w < _numWalls; w++) {
    if (!_map->hasTile(OPEN_MAIN)) break;
    if (tries >= MAX_TRIES) {
      tries = 0;
      continue;
    }
    auto tile = _map->getRandom(OPEN_MAIN, _rand);
    if (_rand.nextFloat() < 0.5f) { // 50% chance of horiz, 50% chance of vert
      xDir = 1;
      yDir = 0;
      invXDir = 0;
      invYDir = 1;
    } else {
      xDir = 0;
      yDir = 1;
      invXDir = 1;
      invYDir = 0;
    }
    // Process into both directions
    auto t1 = KBVector<Tile>(), t2 = KBVector<Tile>();
    KBVector<Tile> dir1 = process(t1, tile, xDir, yDir, invXDir, invYDir);
    KBVector<Tile> dir2 = process(t2, tile, -xDir, -yDir, invXDir, invYDir);
    // Form the full vector which has all tiles into both directions and the initial tile
    KBVector<Tile> line = KBVector<Tile>();
    for (auto i = dir2.length() - 1; i >= 1; i--) line += dir2[i];
    line += tile;
    for (auto i = 1; i < dir1.length(); i++) line += dir1[i];

    if (line.length() < _minWallLen || line.length() > _maxWallLen) {
      tries++;
      w--;
      continue;
    }
    tries = 0;
    auto opening = _rand.nextInt(line.length() - 3) + 1;
    auto wallType = CONSTRUCTED_WALL;
    if (_rand.nextFloat() < 0.3f) wallType = WINDOW;
    for (auto i = 0; i < line.length(); i++) {
      auto type = wallType;
      if (i == opening) {
        if (_rand.nextFloat() < 0.5f || wallType == WINDOW) type = OPEN_MAIN;
        else type = DOOR;
      }
      auto tile = line[i];
      (*_map)[tile.getX()][tile.getY()].setType(type);
    }
  }
}

auto ConstructedWallPlacer::process(KBVector<Tile>& processed, Tile& tile, int xDir, int yDir, int invXDir, int invYDir) -> KBVector<Tile>& {
  const auto x = tile.getX(), y = tile.getY();
   // If we hit a wall, stop
  if (!(*_map)[x][y].isOpen()) return processed;
  processed += tile;
  //If our line touches a wall, stop, but include the last tile
  if (!(*_map)[x + invXDir][y + invYDir].isOpen() || !(*_map)[x - invXDir][y - invYDir].isOpen()) return processed;
  auto next = (*_map)[x + xDir][y + yDir];
  return process(processed, next, xDir, yDir, invXDir, invYDir);
}
