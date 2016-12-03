#include "minebombers/level/FloodFill.hpp"

auto FloodFill::calculateOpen() -> std::shared_ptr<TileMap> {
  const auto width = _map.length();
  const auto height = _map[0].length();
  auto mod = std::make_shared<TileMap>(width, height);
  for (auto x = 0u; x < width; x++) {
    for (auto y = 0u; y < height; y++) {
      auto type = UNDEFINED;
      if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
        type = INDESCTRUCTIBLE_WALL;
      } else if (_map[x][y].isAlive()) {
        type = CAVE_WALL;
      }
      (*mod)[x][y].setType(type);
    }
  }

  _groupCounter = 0;
  auto open = KBVector<Cell>();
  _amounts = KBMap<int, int>();
  for (auto x = 0u; x < width; x++) {
    for (auto y = 0u; y < height; y++) {
      if ((*mod)[x][y].getType() != UNDEFINED || (*mod)[x][y].isGrouped()) continue;
      open += _map[x][y];
      (*mod)[x][y].setGroup(_groupCounter);
      _amounts[_groupCounter] = 1;
      flood(open, mod);
      _groupCounter++;
    }
  }
  int biggestGroup = -1;
  int biggestGroupAmount = -1;
  for (auto group : _amounts.keys()) {
    if (_amounts[group] > biggestGroupAmount) {
      biggestGroup = group;
      biggestGroupAmount = _amounts[group];
    }
  }
  for (auto x = 0u; x < width; x++) {
    for (auto y = 0u; y < height; y++) {
      if ((*mod)[x][y].getType() == UNDEFINED) (*mod)[x][y].setOpen((*mod)[x][y].getGroup() == biggestGroup);
    }
  }
  mod->setGroups(_amounts);
  return mod;
}

auto FloodFill::flood(KBVector<Cell>& open, std::shared_ptr<TileMap>& mod) -> void {
  auto cell = open[0];
  open.remove(0);
  auto neighbors = Cell::getDirectNeighbors(_map, cell._x, cell._y);
  for (auto c : neighbors) {
    if ((*mod)[c._x][c._y].isGrouped() || (*mod)[c._x][c._y].getType() != UNDEFINED) continue;
    (*mod)[c._x][c._y].setGroup(_groupCounter);
    _amounts[_groupCounter]++;
    open += c;
  }
  if (open.length() > 0) flood(open, mod);
}
