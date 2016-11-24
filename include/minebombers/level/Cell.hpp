#pragma once

#include "collection/mutable/KBVector.hpp"

class Cell {
public:
  Cell(uint x, uint y, bool alive): _x(x), _y(y), _alive(alive) {}
  auto isAlive() -> bool { return _alive; }

  static auto getDirectNeighbors(KBVector<KBVector<Cell>>& map, int x, int y) -> KBVector<Cell> {
    auto res = KBVector<Cell>();
    for (auto i = x - 1; i <= x + 1; i++) {
      for (auto j = y - 1; j <= y + 1; j++) {
        if ((i != x && j != y) || (i == x && j == x) || i >= map.length() || j >= map[i].length()) continue;
        res += map[i][j];
      }
    }
    return res;
  }
  uint _x, _y;
private:
  bool _alive;
};
