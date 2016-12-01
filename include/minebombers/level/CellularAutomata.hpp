#pragma once

#include <tuple>
#include "minebombers/level/CellularAutomataPhase.hpp"
#include "collection/mutable/KBMap.hpp"
#include "service/Random.hpp"
#include "minebombers/level/Cell.hpp"

class CellularAutomata {
public:
  CellularAutomata(uint width, uint height, int seed, KBVector<std::shared_ptr<CellularAutomataPhase>>& phases):
    _width(width),
    _height(height),
    _seed(seed),
    _phases(phases)
    {}
  ~CellularAutomata() {}

  auto generate() -> KBVector<KBVector<Cell>>;
private:
  const uint _width, _height, _seed;
  const KBVector<std::shared_ptr<CellularAutomataPhase>>& _phases;
  auto getNearbyLivingAmount(KBVector<KBVector<Cell>>& map, uint x, uint y) -> uint;
};
