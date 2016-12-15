#pragma once

#include "minebombers/level/Cell.hpp"
#include "service/Random.hpp"

/**
 * A superclass for all CellularAutomata-phases. Each phase has n iterations,
 * and a function which defines the next state of a given cell, receiving
 * the previous state and number of living neighbors as a parameter.
 */
class CellularAutomataPhase {
public:
  CellularAutomataPhase() {}
  virtual ~CellularAutomataPhase() {}

  virtual auto nextState(Cell& currentState, uint nearbyLiving) -> Cell = 0;
  virtual auto getIterations() -> uint = 0;
};
/**
 * This phase is used in the beginning of level generation. It outputs a 2d
 * array, where each cell has a random chance to be alive, independent of the
 * previous state or neighbour states.
 */
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

/**
 * The 'standard' cellular automata phase, this phase has an underThreshold and
 * an overThreshold, and the cell is alive in the next phase if it has a number
 * of alive neighbors lower than the underThreshold or higher than overThreshold.
 * Causes very cave like formations with the correct parameters.
 */
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

/**
 * BorderCellularAutomataPhase is used to add a border of alive cells around
 * a cell map. It simply toggles all cells on edge to alive, and leaves all other
 * cells unchanged.
 */
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
