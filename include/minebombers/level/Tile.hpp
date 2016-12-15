#pragma once

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

/**
 * Holds all required data of a single tile. A tile has coordinates, type
 * information of which areaGroup it is and if it belongs to the biggest (=main)
 * area in a tileMap. Funtionaly for mainArea and areaGroup is not defined
 * for non-open areas.
 */
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

  auto isSeeThrough() const -> bool {
    switch(getType()) {
      case INDESCTRUCTIBLE_WALL:
      case CAVE_WALL:
      case CONSTRUCTED_WALL:
        return false;
      default:
        return true;
    }
  }

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
