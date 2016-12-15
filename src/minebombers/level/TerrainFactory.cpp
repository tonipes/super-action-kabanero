#include "minebombers/level/TerrainFactory.hpp"

#include <utility>
#include "minebombers/behaviors/TerrainBehavior.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"
#include "minebombers/util/NodeFactory.hpp"

std::map<TileType, TerrainProperty> TerrainFactory::terrainProperties = {
  { CAVE_WALL, TerrainProperty(8, 100.0f, "tiles/pebble_brown") },
  { INDESCTRUCTIBLE_WALL, TerrainProperty(11, 10000000000.0f, "tiles/stone_brick") },
  { CONSTRUCTED_WALL, TerrainProperty(3, 70.0f, "tiles/rect_gray") },
  { WINDOW, TerrainProperty(0, 10.0f, "tiles/window") }
};

auto TerrainFactory::generateTerrain(
    TileType tileType,
    std::string name,
    b2World& world,
    std::shared_ptr<TileMap>& map) -> std::shared_ptr<Node> {

  auto node = std::make_shared<Node>(name);

  const auto properties = terrainProperties[tileType];
  const auto spriteBaseName = properties.spriteBaseName;
  const auto numVariations = properties.numVariations;
  const auto health = properties.health;

  const auto sprite = getRandomSprite(spriteBaseName, numVariations);
  node->addAttachment(sprite);

  const auto physicsBody = createPhysSquare(world, COLLISION_CATEGORY_WALL, COLLISION_MASK_WALL);
  auto physAttachment = std::make_shared<PhysicsAttachment>(physicsBody);
  node->addAttachment(physAttachment);

  node->addBehavior<TerrainBehavior>(health, map);
  auto material_att = std::make_shared<CollisionMaterialAttachment>();
  material_att->staticMaterial = true;
  node->addAttachment(material_att);
  // node->setBoundingBox(BoundingBox(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f)));

  return node;
}

auto TerrainFactory::createPhysSquare(b2World& world, uint categoryBits, uint maskBits) -> b2Body* {
  b2BodyDef bodyDef;
  b2Body* body = world.CreateBody(&bodyDef);
  b2PolygonShape box;

  box.SetAsBox(0.5f, 0.5f);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &box;
  fixtureDef.density = 1.0f;
  fixtureDef.filter.categoryBits = categoryBits;
  fixtureDef.filter.maskBits = maskBits;


  body->CreateFixture(&fixtureDef);
  return body;
}

auto TerrainFactory::getRandomSprite(
    std::string baseName,
    int numVariations) -> std::shared_ptr<SpriteAttachment> {

  const auto& random = Services::random();
  if (numVariations == -1) {
    return std::make_shared<SpriteAttachment>(baseName);
  }
  std::string spriteName = baseName + std::to_string(random->nextInt(numVariations));
  return std::make_shared<SpriteAttachment>(spriteName);
}
