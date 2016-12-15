#pragma once

#include "scene/3D/Node3D.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "service/Services.hpp"
#include "minebombers/level/Tile.hpp"
#include "minebombers/level/TerrainProperty.hpp"
#include "minebombers/level/TileMap.hpp"

/**
 * Compiles a node-tree of terrain from a tilemap.
 */
class TerrainFactory {
public:
  static auto generateTerrain(
      TileType tileType,
      std::string name,
      b2World& world,
      std::shared_ptr<TileMap>& map) -> std::shared_ptr<Node<Transform3D>>;

  static auto getRandomSprite(
      std::string baseName,
      int numVariations) -> std::shared_ptr<SpriteAttachment>;

private:
  static std::map<TileType, TerrainProperty> terrainProperties;

  static auto createPhysSquare(b2World& world, uint categoryBits, uint maskBits) -> b2Body*;
};
