#pragma once

#include <tuple>
#include "collection/mutable/KBMap.hpp"
#include "service/Random.hpp"

class CellularAutomataPhase {
public:
  CellularAutomataPhase() {}
  virtual ~CellularAutomataPhase() {}

  virtual auto nextState(bool currentState, uint nearbyLiving) -> bool const = 0;
  virtual auto getIterations() -> uint const = 0;
};

class RandomCellularAutomataPhase : public CellularAutomataPhase {
public:
  RandomCellularAutomataPhase(const float chance, Random& rand) :
  _livingChance(chance),
  _rand(rand)
  {}
  ~RandomCellularAutomataPhase() {}

  auto nextState(bool current, uint nearby) -> bool const {
    return _rand.nextFloat() < _livingChance;
  }
  auto getIterations() -> uint const {
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
  ~ThresholdCellularAutomataPhase() {}

  auto nextState(bool current, uint nearby) -> bool const {
    auto total = nearby + (current ? 1u : 0u);
    return total >= _overThreshold || total <= _underThreshold;
  }
  auto getIterations() -> uint const {
    return _iterations;
  }
private:
  const uint _iterations, _underThreshold, _overThreshold;
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

  auto generate() -> KBVector<KBVector<bool>> const;
private:
  const uint _width, _height, _seed;
  const KBVector<std::shared_ptr<CellularAutomataPhase>>& _phases;
  auto getNearbyLivingAmount(KBVector<KBVector<bool>>(), uint x, uint y) -> uint const;
};
