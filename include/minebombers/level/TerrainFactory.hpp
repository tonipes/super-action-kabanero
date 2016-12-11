#pragma once

#include "scene/Node.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "service/Services.hpp"
#include "minebombers/level/Tile.hpp"
#include "minebombers/level/TerrainProperty.hpp"
#include "minebombers/level/TileMap.hpp"
#include <Box2D/Box2D.h>

class TerrainFactory {
public:
  static auto generateTerrain(
      TileType tileType,
      std::string name,
      b2World& world,
      std::shared_ptr<TileMap>& map) -> std::shared_ptr<Node>;

  static auto getRandomSprite(
      std::string baseName,
      int numVariations) -> std::shared_ptr<SpriteAttachment>;

private:
  static std::map<TileType, TerrainProperty> terrainProperties;

  static auto createPhysSquare(b2World& world) -> b2Body*;
};
