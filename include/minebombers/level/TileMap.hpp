#pragma once

#include <iostream>

#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "service/Random.hpp"
#include "minebombers/level/Tile.hpp"

/**
 * A collection of tiles, in the form of width*height 2d vector. contains
 * functions to make accessing and changing the tiles easier, and also
 * helper functions for debugging and/or getting a random tile of certain type
 */
class TileMap {
public:
  TileMap(int width, int height) : _width(width), _height(height) {
    _tiles = KBVector<KBVector<Tile>>();
    for (auto x = 0; x < width; x++) {
      _tiles += KBVector<Tile>();
      for (auto y = 0; y < height; y++) {
        _tiles[x] += Tile(UNDEFINED, x, y, false, -1);
      }
    }
  }

  auto operator[](size_t index) -> KBVector<Tile>& {
    return _tiles[index];
  }

  auto hasTile(TileType ofType) const -> bool {
    return hasTile(ofType, 0, 0, _width, _height);
  }

  auto hasTile(TileType ofType, int left, int top, int right, int bottom) const -> bool {
    for (auto x = left; x < right; x++) {
      for (auto y = top; y < bottom; y++) {
        if (_tiles[x][y].getType() == ofType) return true;
      }
    }
    return false;
  }

  auto getRandom(TileType ofType, Random& rand) -> Tile {
    return getRandom(ofType, rand, 0, 0, _width, _height);
  }

  auto getRandom(TileType ofType, Random& rand, int left, int top, int right, int bottom) -> Tile {
    while (true) {
      auto x = rand.nextInt(right - left) + left, y = rand.nextInt(bottom - top) + top;
      auto candidate = _tiles[x][y];
      if (candidate.getType() == ofType) return candidate;
    }
  }

  auto setGroups(KBMap<int, int> am) -> void { _groupAmounts = am; }
  auto getGroups() -> KBMap<int, int> { return _groupAmounts; }

  auto getWidth() -> int { return _width; }
  auto getHeight() -> int { return _height; }

  auto debugPrint(std::ostream& stream) -> void {
    for (auto x = 0; x < getWidth(); x++) {
      for (auto y = 0; y < getHeight(); y++) {
        std::cout << _tiles[x][y].getCharRepresentation();
      }
      std::cout << std::endl;
    }
  }

private:
  int _width, _height;
  KBMap<int, int> _groupAmounts;
  KBVector<KBVector<Tile>> _tiles;
};
