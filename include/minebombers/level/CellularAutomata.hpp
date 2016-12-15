#pragma once

#include <tuple>
#include "minebombers/level/CellularAutomataPhase.hpp"
#include "collection/mutable/KBMap.hpp"
#include "service/Random.hpp"
#include "minebombers/level/Cell.hpp"

/**
 * CellularAutomata is a way of generating 2d boolean arrays.
 * It functions in the same way as the famous Game of Life, with different
 * parametrizable iterations. Each iteration outputs a new 2d array of dead
 * and alive cells, based on the previous array and given parameters.
 * CellularAutomata contains only the core logic for executing different
 * iterations, called phases, and the rules are implemented in individual phases.
 */
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
