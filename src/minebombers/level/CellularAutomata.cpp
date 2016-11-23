#include "minebombers/level/CellularAutomata.hpp"

auto CellularAutomata::generate() -> KBVector<KBVector<Cell>> {
  auto map = KBVector<KBVector<Cell>>();
  for (auto phase : _phases) {
    for (auto i = 0u; i < phase->getIterations(); i++) {
      auto result = KBVector<KBVector<Cell>>();
      for (auto x = 0u; x < _width; x++) {
        result += KBVector<Cell>();
        for (auto y = 0u; y < _height; y++) {
            auto nearbyWalls = getNearbyLivingAmount(map, x, y);
            if (map.isEmpty()) {
              auto temp = Cell(false);
              result[x] += phase->nextState(temp, nearbyWalls);
            } else {
              result[x] += phase->nextState(map[x][y], nearbyWalls);
            }
        }
      }
      map = result;
    }
  }
  return map;
}

auto CellularAutomata::getNearbyLivingAmount(KBVector<KBVector<Cell>>& map, uint x, uint y) -> uint {
  auto num = 0u;
  for (auto i = x - 1; i <= x + 1; i++) {
    for (auto j = y - 1; j <= y + 1; j++) {
      if ((i == x && j == y) || i >= map.length() || j >= map[i].length()) continue;
      num += (map[i][j].isAlive() ? 1 : 0);
    }
  }
  return num;
}
