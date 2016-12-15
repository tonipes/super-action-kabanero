#pragma once

#include "collection/mutable/KBVector.hpp"
#include "minebombers/level/CellularAutomata.hpp"
#include "minebombers/level/SpawnPointPlacer.hpp"
#include "minebombers/level/ConstructedWallPlacer.hpp"
#include "minebombers/level/FloodFill.hpp"
#include "random/StdLibRandom.hpp"
/**
 * CaveGenerator is the collection of all required algoritms to produce the
 * TileMap used in the game. It uses a Random, CellularAutomata, FloodFill,
 * SpawnPointPlacer and ConstructedWallPlacer, and also checks if the map
 * contains a large enough main area. The output from generate must be fed
 * into LevelCompiler to output usable node-tree.
 */
class CaveGenerator {
public:
  CaveGenerator(int seed, uint width, uint height, int playerSectors, int numItems) :
  _seed(seed), _width(width), _height(height), _playerSectors(playerSectors), _numItems(numItems) {}

  auto generate() -> std::shared_ptr<TileMap> {
    auto random = StdLibRandom(); //TODO
    random.seed(_seed);

    // TODO read phases from config
    auto phases = KBVector<std::shared_ptr<CellularAutomataPhase>>();
    phases += std::make_shared<RandomCellularAutomataPhase>(0.45f, random);
    phases += std::make_shared<ThresholdCellularAutomataPhase>(3, 3, 4);
    phases += std::make_shared<ThresholdCellularAutomataPhase>(4, 0, 4);

    auto mata = CellularAutomata(_width, _height, 2, phases);
    auto cellData = mata.generate();

    auto tileMap = FloodFill(cellData).calculateOpen();

    int minimumOpen = _width * _height * 45 / 100; // We want at least X% to be open & main area, or we discard the map
    bool enoughOpen = false;
    for (auto am : tileMap->getGroups().values()) {
      if (am >= minimumOpen) enoughOpen = true;
    }
    if (!enoughOpen) {
      // Change seed for a different map, the recursively generate again until enough open is found
      _seed = random.nextInt(2147483647);
      return generate();
    }
    SpawnPointPlacer(tileMap, _playerSectors, _numItems, random).run();
    ConstructedWallPlacer(tileMap, 32, 3, 12, random).run();

    return tileMap;
  }

private:
  int _seed, _playerSectors, _numItems;
  uint _width, _height;
};
