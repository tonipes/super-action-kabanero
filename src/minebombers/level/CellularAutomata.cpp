#include "minebombers/level/CellularAutomata.hpp"

auto CellularAutomata::generate() -> KBVector<KBVector<bool>> const {
  auto map = KBVector<KBVector<bool>>();
  for (auto phase : _phases) {
    for (auto i = 0u; i < phase.getIterations(); i++) {
      auto result = KBVector<KBVector<bool>>();
      for (auto x = 0u; x < _width; x++) {
        result += KBVector<bool>();
        for (auto y = 0u; y < _height; y++) {
            auto nearbyWalls = getNearbyLivingAmount(map, x, y);
            auto nextState = phase.nextState(map[x][y], nearbyWalls);
            result[x] += nextState;
        }
      }
      map = result;
    }
  }
}

auto CellularAutomata::getNearbyLivingAmount(KBVector<KBVector<bool>>() map, uint x, uint y) -> uint const {
  auto num = 0u;
  for (auto i = x - 1; i <= x + 1; i++) {
    for (auto j = y - 1; j <= y + 1; j++) {
      if ((i == x && j == y) || i < 0 || j < 0 || i >= map.length() || j >= map[i].length()) continue;
      num += (map[i][j] ? 1 : 0);
    }
  }
  return num;
}
