#pragma once

#include "minebombers/level/TileMap.hpp"
#include "minebombers/level/FogMap.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "physics/CollisionData.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "graphics/effects/CircleEffect.hpp"
#include "minebombers/behaviors/TerrainBehaviour.hpp"
#include "minebombers/behaviors/FogBehaviour.hpp"
#include "minebombers/behaviors/PlayerBehaviour.hpp"
#include "minebombers/behaviors/WallBehavior.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"

// #include "minebombers/attachments/GunAttachment.hpp"
#include "minebombers/data/GunParameters.hpp"
#include "minebombers/behaviors/ItemNodeBehaviour.hpp"
#include <sstream>

class LevelCompiler {
public:
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
    ground->setLocalPosition(glm::vec3(0,0,0));
    level->addChild(ground);

    auto obj = std::make_shared<Node<Transform3D>>("obj");
    obj->setLocalPosition(glm::vec3(0,0,2));
    level->addChild(obj);


    auto items = normalGuns;
    auto artifacts = artifactGuns;
    for (auto x = 0; x < map->getWidth(); x++) {
      for (auto y = 0; y < map->getHeight(); y++) {
        auto tileNode =  std::make_shared<Node<Transform3D>>(name("tile", x, y));
        auto floorNode = std::make_shared<Node<Transform3D>>(name("floor", x, y));

        // ground->addChild(floorNode);
        obj->addChild(tileNode);

        // floorNode->addAttachment(getSprite("tiles/dirt", 2));
        // floorNode->setLocalPosition(glm::vec3(x, y, 0));
        //
        // floorNode->setSleep(true);
        // floorNode->setLocalPosition(glm::vec3(x, y, -2));
        auto item = items[(x+y)%items.length()];
        auto art = artifacts[(x+y)%artifacts.length()];

        switch ((*map)[x][y].getType()) {
          case CAVE_WALL:
            tileNode->addChild(getTerrain("tiles/pebble_brown", 8, 100.0f, x, y, map));
            break;
          case INDESCTRUCTIBLE_WALL:
            tileNode->addChild(getTerrain("tiles/stone_brick", 11, 10000000000.0f, x, y, map));
            break;
          case CONSTRUCTED_WALL:
            tileNode->addChild(getTerrain("tiles/rect_gray", 3, 70.0f, x, y, map));
            break;
          case WINDOW:
            tileNode->addChild(getTerrain("tiles/window", 0, 10.0f, x, y, map));
            break;
          case ITEM_LOCATION:
            tileNode->addChild(getItem(item, x, y));
            break;
          case ARTIFACT_LOCATION:
            tileNode->addChild(getItem(art, x, y));
            break;
        }
      }
    }
  }

  auto initFog(
    std::shared_ptr<TileMap> map, std::shared_ptr<FogMap> fogMap,
    std::shared_ptr<Node<Transform3D>> root
  ) -> void {

    fogMap->init(map->getWidth(), map->getHeight());
    auto fogNode = std::make_shared<Node<Transform3D>>("fog");
    root->addChild(fogNode);

    fogNode->setLocalPosition(glm::vec3(0,0,100));
    for (auto x = 0; x < map->getWidth(); x++) {
      for (auto y = 0; y < map->getHeight(); y++) {
        auto node = std::make_shared<Node<Transform3D>>(name("fog", x, y));
        node->addAttachment(getSprite("tiles/fog", -1));
        node->setLocalPosition(glm::vec3(x,y,0));
        node->setSleep(true);
        fogNode->addChild(node);
        (*fogMap)[x][y] = node;
      }
    }
    fogNode->addBehavior<FogBehaviour>(map, fogMap);
  }

  auto materializePlayer(
    std::shared_ptr<TileMap> map,
    std::shared_ptr<Node<Transform3D>> root
  ) -> void{
    auto tile = map->getRandom(PLAYER_SPAWN_POINT, _rand);

    auto node = std::make_shared<Node<Transform3D>>("player");
    node->setLocalPosition(glm::vec3(tile.getX(), tile.getY(), 2));
    node->addAttachment(getSprite("tiles/spriggan_druid", -1));

    // auto effect_att = std::make_shared<EffectAttachment>(std::make_shared<CircleEffect>(32.0f));
    // node->addAttachment(effect_att);

    auto material_att = std::make_shared<CollisionMaterialAttachment>();
    material_att->bulletRebound = true;
    material_att->isPlayer = true;

    node->addAttachment(material_att);
    auto gun = std::make_shared<GunAttachment>(
      normalGuns[0] //std::make_shared<GunParameters>(5.0f, 1.0f, 1, 0.4f, 8.0f, "tiles/sniper_normal")
    );

    node->addAttachment(gun);

    auto physCircle = createPhysCircle(tile.getX(), tile.getY());
    auto physAttachment = std::make_shared<PhysicsAttachment>(physCircle);

    node->addAttachment(physAttachment);

    node->addBehavior<PlayerBehaviour>();
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
    body->CreateFixture(&circleShape, 1.0f);
    return body;
  }
private:
  Random& _rand;
  b2World& _world;
  auto getSprite(std::string baseName, int variations) -> std::shared_ptr<SpriteAttachment> {
      if (variations == -1) {
        return std::make_shared<SpriteAttachment>(baseName);
      }
      std::ostringstream oss;
      oss << baseName << _rand.nextInt(variations);
      return std::make_shared<SpriteAttachment>(oss.str());
  }
  auto name(std::string base, int x, int y) -> std::string {
    std::ostringstream oss;
    oss << base << x << "-" << y;
    return oss.str();
  }
  auto getTerrain(std::string sprites, int spriteVar, float health, int x, int y, std::shared_ptr<TileMap> map) -> std::shared_ptr<Node<Transform3D>> {
    auto node = std::make_shared<Node<Transform3D>>(name("obj",x,y));
    node->setLocalPosition(glm::vec3(x, y, 0));
    node->addAttachment(getSprite(sprites, spriteVar));

    //node->addBehavior<WallBehavior>();
    auto terrainBehaviour = node->addBehavior<TerrainBehaviour>(health, map, x, y);
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->staticMaterial = true;

    node->addAttachment(material_att);

    auto physBody = createPhysSquare(x, y);

    auto physAttachment = std::make_shared<PhysicsAttachment>(physBody);
    node->addAttachment(physAttachment);

    return node;
  }

  auto getItem(std::shared_ptr<GunParameters> gun, int x, int y) -> std::shared_ptr<Node<Transform3D>> {
    auto node = std::make_shared<Node<Transform3D>>(name("item",x,y));
    node->setLocalPosition(glm::vec3(x,y,0));
    node->addAttachment(getSprite(gun->sprite, -1));

    auto itBeh = node->addBehavior<ItemNodeBehaviour>();
    auto material_att = std::make_shared<CollisionMaterialAttachment>();
    material_att->gunParameters = Some<std::shared_ptr<GunParameters>>(gun);
    // material_att->itemLink = itBeh;
    node->addAttachment(material_att);

    auto physBody = createPhysSquare(x, y);

    auto collisionData = new CollisionData("", material_att);
    physBody->SetUserData(collisionData);

    auto physAttachment = std::make_shared<PhysicsAttachment>(physBody);
    node->addAttachment(physAttachment);

    return node;
  }
};

KBVector<std::shared_ptr<GunParameters>> LevelCompiler::normalGuns {
  std::make_shared<GunParameters>(150.0f,  0.3f, 1, 0.1f, 10.0f, "tiles/pistol_normal",   "test-effect/crystal_spear0", "rocket_launch.ogg", true, 3.0f, 0),
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
