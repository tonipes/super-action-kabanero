#pragma once

#include <iostream>

#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "service/Random.hpp"

enum TileType {
  UNDEFINED,
  INDESCTRUCTIBLE_WALL,
  CAVE_WALL,
  CONSTRUCTED_WALL,
  WINDOW,
  DOOR,
  OPEN_MAIN,
  OPEN_SIDE,
  PLAYER_SPAWN_POINT,
  ITEM_LOCATION,
  ARTIFACT_LOCATION,
};

class Tile {
public:
  Tile(TileType type, int x, int y, bool main, int group) :
  _type(type), _x(x), _y(y), _mainArea(main), _areaGroup(group)
  {}

  auto getType() const -> TileType { return _type; }
  auto setType(TileType t) -> void { _type = t; }
  auto getX() const -> int { return _x; }
  auto getY() const -> int { return _y; }
  auto isMainArea() const -> bool { return _type == OPEN_MAIN; }
  auto setOpen(bool main) -> void { if (main) _type = OPEN_MAIN; else _type = OPEN_SIDE; }
  auto getGroup() const -> int { return _areaGroup; }
  auto setGroup(int i) -> void { _areaGroup = i; }
  auto isGrouped() const -> bool { return _areaGroup >= 0; }

  auto isOpen() const -> bool {
    switch (getType()) {
      case OPEN_MAIN:
      case OPEN_SIDE:
      case PLAYER_SPAWN_POINT:
      case ITEM_LOCATION:
        return true;
      default:
        return false;
      }
  }

  auto getCharRepresentation() const -> char {
    switch (getType()) {
      case UNDEFINED: return '?';
      case INDESCTRUCTIBLE_WALL: return '$';
      case CAVE_WALL: return '#';
      case CONSTRUCTED_WALL: return 'X';
      case WINDOW: return 'O';
      case DOOR: return '+';
      case OPEN_MAIN: return '.';
      case OPEN_SIDE: return ' ';
      case PLAYER_SPAWN_POINT: return '@';
      case ITEM_LOCATION: return 'I';
      case ARTIFACT_LOCATION: return 'A';
      default: return '!'; // If you get this, you haven't defined a case for a new tile type
    }
  }

private:
  TileType _type;
  int _x, _y;
  bool _mainArea;
  int _areaGroup;
};

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
