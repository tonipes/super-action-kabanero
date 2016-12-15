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
#include "minebombers/behaviors/PlayerBehavior.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"

#include "minebombers/data/GunParameters.hpp"
#include "minebombers/behaviors/ItemNodeBehaviour.hpp"
#include "minebombers/nodes/Player.hpp"
#include <sstream>

class LevelCompiler {
public:
  static constexpr float CHUNK_SIZE = 8.0f;

  LevelCompiler(Random& rand, b2World& w): _rand(rand), _world(w) {}

  static KBVector<std::shared_ptr<GunParameters>> normalGuns;
  static KBVector<std::shared_ptr<GunParameters>> artifactGuns;

  auto materializeLevel(
    std::shared_ptr<TileMap> map,
    std::shared_ptr<Node> root) -> void {

    auto level = std::make_shared<Node>("level");
    level->setSleep(true);
    root->addChild(level);

    auto ground = std::make_shared<Node>("ground");
    ground->setLocalPosition(glm::vec3(0,0,-200));
    ground->setSleep(true);
    level->addChild(ground);

    auto obj = std::make_shared<Node>("obj");
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
        auto chunkNode = std::make_shared<Node>(name("chunk", xChunk, yChunk));
        chunkNode->setLocalPosition(glm::vec3(xChunk * CHUNK_SIZE, yChunk * CHUNK_SIZE, 0));
        chunkNode->setSleep(true);
        obj->addChild(chunkNode);

        auto groundChunk = std::make_shared<Node>(name("ground-chunk", xChunk, yChunk));
        groundChunk->setLocalPosition(glm::vec3(xChunk * CHUNK_SIZE, yChunk * CHUNK_SIZE, 0));
        groundChunk->setSleep(true);
        ground->addChild(groundChunk);
        for (auto x = 0; x < CHUNK_SIZE; x++) {
          for (auto y = 0; y < CHUNK_SIZE; y++) {
            auto totalX = (int)(xChunk * CHUNK_SIZE + x);
            auto totalY = (int)(yChunk * CHUNK_SIZE + y);

            auto groundNode = std::make_shared<Node>(name("ground", totalX, totalY));
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

  auto materializePlayer(
      std::shared_ptr<TileMap> map,
      std::shared_ptr<Node> root,
      int playerNumber) -> void {

    auto tile = map->getRandom(PLAYER_SPAWN_POINT, _rand);

    auto playerId = "player" + std::to_string(playerNumber);

    auto node = Player::create(playerId, _world);

    auto gun = std::make_shared<GunAttachment>(normalGuns[0]);
    node->addAttachment(gun);

    node->setLocalPosition(glm::vec3(tile.getX(), tile.getY(), node->position().z));
    const auto& physAttachment = node->get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {
      phys.setPosition(tile.getX(), tile.getY());
    });

    root->addChild(node);
  }

  auto createPhysSquare(float x, float y) -> b2Body* {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    b2Body* body = _world.CreateBody(&bodyDef);
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);
    body->CreateFixture(&box, 0.0f);
    return body;
  }
  auto createPhysCircle(float x, float y) -> b2Body* {
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
    // fixtureDef.filter.groupIndex = -1;
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

  auto getItem(std::shared_ptr<GunParameters> gun, int x, int y) -> std::shared_ptr<Node> {
    auto node = std::make_shared<Node>(name("item",x,y));
    node->addAttachment(getSprite(gun->sprite, -1));

    auto itBeh = node->addBehavior<ItemNodeBehaviour>();
    auto material_att = std::make_shared<CollisionMaterialAttachment>();
    material_att->gunParameters = Some<std::shared_ptr<GunParameters>>(gun);
    // material_att->itemLink = itBeh;
    node->addAttachment(material_att);

    auto physBody = createPhysSquare(0, 0);

    auto collisionData = new CollisionData("", material_att);
    physBody->SetUserData(collisionData);

    auto physAttachment = std::make_shared<PhysicsAttachment>(physBody);
    node->addAttachment(physAttachment);

    return node;
  }
};

KBVector<std::shared_ptr<GunParameters>> LevelCompiler::normalGuns {
  std::make_shared<GunParameters>(150.0f,  2.0f, 1, 0.1f, 10.0f, "tiles/pistol_normal",   "test-effect/crystal_spear0", "rocket_launch.ogg", true, 1.0f, 0),
  std::make_shared<GunParameters>(  8.0f,  6.0f, 1, 0.3f, 12.0f, "tiles/rifle_normal",    "test-effect/crystal_spear0", "gunshot.ogg"),
  std::make_shared<GunParameters>( 10.0f,  1.5f, 3, 0.2f, 10.0f, "tiles/shotgun_normal",  "test-effect/crystal_spear0", "gunshot.ogg"),
  std::make_shared<GunParameters>( 60.0f,  0.5f, 1, 0.0f, 25.0f, "tiles/sniper_normal",   "test-effect/crystal_spear0", "gunshot.ogg"),
  std::make_shared<GunParameters>( 60.0f,  0.5f, 1, 0.0f, 25.0f, "tiles/sniper_normal",   "test-effect/crystal_spear0", "gunshot.ogg")
};

KBVector<std::shared_ptr<GunParameters>> LevelCompiler::artifactGuns {
  std::make_shared<GunParameters>( 45.0f,  2.0f, 1, 0.05f, 20.0f, "tiles/pistol_artifact",  "test-effect/crystal_spear0", "gunshot.ogg"),
  std::make_shared<GunParameters>( 20.0f, 10.0f, 1, 0.25f, 20.0f, "tiles/rifle_artifact",   "test-effect/crystal_spear0", "gunshot.ogg"),
  std::make_shared<GunParameters>( 15.0f,  2.5f, 5, 0.15f, 15.0f, "tiles/shotgun_artifact", "test-effect/crystal_spear0", "gunshot.ogg"),
  std::make_shared<GunParameters>(140.0f, 0.75f, 1, 0.00f, 45.0f, "tiles/sniper_artifact",  "test-effect/crystal_spear0", "gunshot.ogg")
};
