#pragma once

#include "minebombers/level/TileMap.hpp"
#include "minebombers/level/FogMap.hpp"
#include "minebombers/level/TerrainFactory.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "physics/CollisionData.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"
#include "graphics/effects/CircleEffect.hpp"
#include "minebombers/behaviors/TerrainBehavior.hpp"
#include "minebombers/behaviors/PlayerBehaviour.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "minebombers/util/NodeFactory.hpp"

#include "minebombers/data/GunParameters.hpp"
#include "minebombers/behaviors/ItemNodeBehaviour.hpp"
#include <sstream>

/**
 * A collection of different functions and utils to 'compile' the level from
 * raw tilemap data into nodes, attachments, behaviours and physics objects.
 */
class LevelCompiler {
public:
  static constexpr float CHUNK_SIZE = 8.0f;

  LevelCompiler(Random& rand, b2World& w): _rand(rand), _world(w) {}

  static KBVector<std::shared_ptr<GunParameters>> normalGuns;
  static KBVector<std::shared_ptr<GunParameters>> artifactGuns;

  auto materializeLevel(
    std::shared_ptr<TileMap> map,
    std::shared_ptr<Node<Transform3D>> root
  ) -> void {

    auto level = std::make_shared<Node<Transform3D>>("level");
    level->setSleep(true);
    root->addChild(level);

    auto ground = std::make_shared<Node<Transform3D>>("ground");
    ground->setLocalPosition(glm::vec3(0,0,-200));
    ground->setSleep(true);
    level->addChild(ground);

    auto obj = std::make_shared<Node<Transform3D>>("obj");
    obj->setLocalPosition(glm::vec3(0,0,-100));
    obj->setSleep(true);
    level->addChild(obj);

    const auto mapWidth = map->getWidth();
    const auto mapHeight = map->getHeight();

    const auto xChunks = ceil(mapWidth / CHUNK_SIZE);
    const auto yChunks = ceil(mapHeight / CHUNK_SIZE);

    const auto groundSprite = std::make_shared<SpriteAttachment>("tiles/dirt0");

    for (auto xChunk = 0; xChunk < xChunks; xChunk++) {
      for (auto yChunk = 0; yChunk < yChunks; yChunk++) {
        auto chunkNode = std::make_shared<Node<Transform3D>>(name("chunk", xChunk, yChunk));
        chunkNode->setLocalPosition(glm::vec3(xChunk * CHUNK_SIZE, yChunk * CHUNK_SIZE, 0));
        chunkNode->setSleep(true);
        obj->addChild(chunkNode);

        auto groundChunk = std::make_shared<Node<Transform3D>>(name("ground-chunk", xChunk, yChunk));
        groundChunk->setLocalPosition(glm::vec3(xChunk * CHUNK_SIZE, yChunk * CHUNK_SIZE, 0));
        groundChunk->setSleep(true);
        ground->addChild(groundChunk);
        for (auto x = 0; x < CHUNK_SIZE; x++) {
          for (auto y = 0; y < CHUNK_SIZE; y++) {
            auto totalX = (int)(xChunk * CHUNK_SIZE + x);
            auto totalY = (int)(yChunk * CHUNK_SIZE + y);

            auto groundNode = std::make_shared<Node<Transform3D>>(name("ground", totalX, totalY));
            // groundNode->setBoundingBox(BousndingBox(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f)));
            chunkNode->addChild(groundNode);
            groundNode->setLocalPosition(glm::vec3(x, y, -100));
            groundNode->addAttachment(groundSprite);
            groundNode->setSleep(true);

            // groundChunk->addChild(groundNode);

            auto item = normalGuns[(totalX+totalY)%normalGuns.length()];
            auto artifact = artifactGuns[(totalX+totalY)%artifactGuns.length()];

            auto terrainType = (*map)[totalX][totalY].getType();

            switch (terrainType) {
              case CAVE_WALL:
              case INDESCTRUCTIBLE_WALL:
              case CONSTRUCTED_WALL:
              case WINDOW: {
                const auto terrain = TerrainFactory::generateTerrain(
                  terrainType, name("terrain", totalX, totalY), _world, map);
                chunkNode->addChild(terrain);
                terrain->setLocalPosition(glm::vec3(x, y, -10));
                const auto pos = terrain->position();
                const auto& physAttachment = terrain->get<PhysicsAttachment>();
                physAttachment.foreach([&](auto phys) {
                  phys.setPosition(pos.x, pos.y);
                });
                break;
              }
              case ITEM_LOCATION: {
                const auto itemNode = getItem(item, totalX, totalY);
                itemNode->setLocalPosition(glm::vec3(x, y, -10));
                chunkNode->addChild(itemNode);
                const auto pos = itemNode->position();
                const auto& physAttachment = itemNode->get<PhysicsAttachment>();
                physAttachment.foreach([&](auto phys) {
                  phys.setPosition(pos.x, pos.y);
                });
                break;
              }
              case ARTIFACT_LOCATION:{
                const auto itemNode = getItem(artifact, totalX, totalY);
                itemNode->setLocalPosition(glm::vec3(x, y, -10));
                chunkNode->addChild(itemNode);
                const auto pos = itemNode->position();
                const auto& physAttachment = itemNode->get<PhysicsAttachment>();
                physAttachment.foreach([&](auto phys) {
                  phys.setPosition(pos.x, pos.y);
                });
                break;
              }
            }
          }
        }
      }
    }
  }

  auto materializeEnemies(
    std::shared_ptr<TileMap> map,
    std::shared_ptr<Node<Transform3D>> root,
    int count,
    float dfficulty)
  -> void  {
    for(auto i = 0; i < count; i++){
      auto tileType = OPEN_MAIN;
      if(_rand.nextInt(1) == 0) tileType = OPEN_SIDE;
      auto tile = map->getRandom(OPEN_MAIN, _rand);
      auto node = NodeFactory::createRandomEnemy(dfficulty, _rand.nextInt(3));

      node->setLocalPosition(glm::vec3(tile.getX(), tile.getY(), 2));
      auto physCircle = createPhysCircle(tile.getX(), tile.getY(), COLLISION_CATEGORY_BULLET, COLLISION_MASK_BULLET);
      auto physAttachment = std::make_shared<PhysicsAttachment>(physCircle);
      node->addAttachment(physAttachment);

      root->addChild(node);
    }

  }

  auto materializePlayer(
    std::shared_ptr<TileMap> map,
    std::shared_ptr<Node<Transform3D>> root,
    int playerId
  ) -> void {
    auto tile = map->getRandom(PLAYER_SPAWN_POINT, _rand);

    std::shared_ptr<Node<Transform3D>> node;
    std::shared_ptr<b2BodyDef> bodyDef;
    std::shared_ptr<b2FixtureDef> fixtureDef;

    std::tie(node, bodyDef, fixtureDef) = NodeFactory::createPlayer(playerId, 3, glm::vec2(tile.getX(), tile.getY()));

    b2Body* body = _world.CreateBody(bodyDef.get());
    body->CreateFixture(fixtureDef.get());

    auto physAttachment = std::make_shared<PhysicsAttachment>(body);
    node->addAttachment(physAttachment);

    root->addChild(node);
  }

  auto createPhysSquare(float x, float y, uint categoryBits, uint maskBits) -> b2Body* {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    b2Body* body = _world.CreateBody(&bodyDef);

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

  auto createPhysCircle(float x, float y, uint categoryBits, uint maskBits) -> b2Body* {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.allowSleep = false;
    b2Body* body = _world.CreateBody(&bodyDef);
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = 0.35f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;

    fixtureDef.density = 1.0f;
    body->CreateFixture(&fixtureDef);
    return body;
  }
private:
  Random& _rand;
  b2World& _world;
  auto getSprite(std::string baseName, int numVariations) -> std::shared_ptr<SpriteAttachment> {
    if (numVariations == -1) {
      return std::make_shared<SpriteAttachment>(baseName);
    }
    std::ostringstream oss;
    oss << baseName << _rand.nextInt(numVariations);
    return std::make_shared<SpriteAttachment>(oss.str());
  }
  auto name(std::string base, int x, int y) -> std::string {
    std::ostringstream oss;
    oss << base << x << "-" << y;
    return oss.str();
  }

  auto getItem(std::shared_ptr<GunParameters> gun, int x, int y) -> std::shared_ptr<Node<Transform3D>> {
    auto node = std::make_shared<Node<Transform3D>>(name("item",x,y));
    node->addAttachment(getSprite(gun->sprite, -1));

    auto itBeh = node->addBehavior<ItemNodeBehaviour>();
    auto material_att = std::make_shared<CollisionMaterialAttachment>();
    material_att->gunParameters = Some<std::shared_ptr<GunParameters>>(gun);
    // material_att->itemLink = itBeh;
    node->addAttachment(material_att);

    auto physBody = createPhysSquare(0, 0, COLLISION_CATEGORY_PICKUP, COLLISION_MASK_PICKUP);

    auto collisionData = new CollisionData("", material_att);
    physBody->SetUserData(collisionData);

    auto physAttachment = std::make_shared<PhysicsAttachment>(physBody);
    node->addAttachment(physAttachment);

    return node;
  }
};
