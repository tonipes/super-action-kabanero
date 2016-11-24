#pragma once

#include <tuple>
#include "collection/mutable/KBMap.hpp"
#include "service/Random.hpp"
#include "minebombers/level/Cell.hpp"

class CellularAutomataPhase {
public:
  CellularAutomataPhase() {}
  virtual ~CellularAutomataPhase() {}

  virtual auto nextState(Cell& currentState, uint nearbyLiving) -> Cell = 0;
  virtual auto getIterations() -> uint = 0;
};

class RandomCellularAutomataPhase : public CellularAutomataPhase {
public:
  RandomCellularAutomataPhase(const float chance, Random& rand) :
  _livingChance(chance),
  _rand(rand)
  {}

  auto nextState(Cell& current, uint nearby) -> Cell {
    auto cell = Cell(current._x, current._y, _rand.nextFloat() < _livingChance);
    return cell;
  }
  auto getIterations() -> uint {
    return 1;
  }
private:
  const float _livingChance;
  Random& _rand;
};

class ThresholdCellularAutomataPhase : public CellularAutomataPhase {
public:
  ThresholdCellularAutomataPhase(uint iterations, uint underThreshold, uint overThreshold) :
  _iterations(iterations),
  _underThreshold(underThreshold),
  _overThreshold(overThreshold)
  {}

  auto nextState(Cell& current, uint nearby) -> Cell {
    auto total = nearby;
    return Cell(current._x, current._y, total > _overThreshold || total < _underThreshold);
  }
  auto getIterations() -> uint {
    return _iterations;
  }
private:
  const uint _iterations, _underThreshold, _overThreshold;
};

class BorderCellularAutomataPhase : public CellularAutomataPhase {
public:
  BorderCellularAutomataPhase(uint width, uint height) :
  _width(width), _height(height)
  {}

  auto nextState(Cell& current, uint nearby) -> Cell {
    return Cell(current._x, current._y, current.isAlive() || current._x == _width - 1 || current._x == 0 || current._y == _height - 1 || current._y == 0);
  }
  auto getIterations() -> uint {
    return 1;
  }
private:
  const uint _width, _height;
};

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
