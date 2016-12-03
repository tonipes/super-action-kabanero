#include "minebombers/level/SpawnPointPlacer.hpp"

auto SpawnPointPlacer::run() -> void {
  for (auto i = 0; i < _playerSectors; i++) {
    auto left = _map->getWidth() * i / _playerSectors;
    auto right = _map->getWidth() * (i + 1) / _playerSectors - 1;
    auto centerX = left + (right - left) / 2;
    for (auto j = 0; j < _playerSectors; j++) {
      auto top = _map->getHeight() * j / _playerSectors;
      auto bottom = _map->getHeight() * (j + 1) / _playerSectors - 1;
      auto centerY = top + (bottom - top) / 2;
      if (_map->hasTile(OPEN_MAIN, left, top, right, bottom)) {
        auto t = _map->getRandom(OPEN_MAIN, _rand, left, top, right, bottom);
        auto t2 = _map->getRandom(OPEN_MAIN, _rand, left, top, right, bottom);
        // We want to gravitate spawn points towards the center of the sectors
        int x = t.getX(), y = t.getY();
        if (abs(t2.getX() - centerX) + abs(t2.getY() - centerY) < abs(x - centerX) + abs(y - centerY)) {
          x = t2.getX();
          y = t2.getY();
        }
        (*_map)[x][y].setType(PLAYER_SPAWN_POINT);
      }
    }
  }
  for (auto i = 0; i < _itemSectors; i++) {
    auto left = _map->getWidth() * i / _itemSectors;
    auto right = _map->getWidth() * (i + 1) / _itemSectors - 1;
    for (auto j = 0; j < _itemSectors; j++) {
      auto top = _map->getHeight() * j / _itemSectors;
      auto bottom = _map->getHeight() * (j + 1) / _itemSectors - 1;
      if (_map->hasTile(OPEN_SIDE, left, top, right, bottom)) {
        auto t = _map->getRandom(OPEN_SIDE, _rand, left, top, right, bottom);
        (*_map)[t.getX()][t.getY()].setType(ARTIFACT_LOCATION);
      }
      if (_map->hasTile(OPEN_MAIN, left, top, right, bottom)) {
        auto t = _map->getRandom(OPEN_MAIN, _rand, left, top, right, bottom);
        (*_map)[t.getX()][t.getY()].setType(ITEM_LOCATION);
      }
    }
  }
}
