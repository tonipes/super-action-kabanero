#pragma once

#include "minebombers/level/TileMap.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "minebombers/behaviors/TerrainBehaviour.hpp"
#include "minebombers/behaviors/PlayerBehaviour.hpp"
#include <sstream>

class LevelCompiler {
public:
  LevelCompiler(Random& rand, b2World& w): _rand(rand), _world(w) {}
  auto materializeLevel(TileMap& map) -> std::shared_ptr<Node<Transform3D>> {
    auto level = std::make_shared<Node<Transform3D>>("level");
    auto ground = std::make_shared<Node<Transform3D>>("ground");
    ground->setLocalPosition(glm::vec3(0,0,0));
    auto obj = std::make_shared<Node<Transform3D>>("obj");
    obj->setLocalPosition(glm::vec3(0,0,2));
    for (auto x = 0; x < map.getWidth(); x++) {
      for (auto y = 0; y < map.getHeight(); y++) {
        auto tileNode =  std::make_shared<Node<Transform3D>>(name("tile", x, y));
        auto floorNode = std::make_shared<Node<Transform3D>>(name("floor", x, y));
        floorNode->addAttachment(getSprite("tiles/dirt", 2));
        floorNode->setLocalPosition(glm::vec3(x, y, 0));
        switch (map[x][y].getType()) {
          case CAVE_WALL :
            tileNode->addChild(getTerrain("tiles/pebble_brown", 8, 100.0f, x, y));
            break;
          case INDESCTRUCTIBLE_WALL :
            tileNode->addChild(getTerrain("tiles/stone_brick", 11, 10000000.0f, x, y));
            break;
          case CONSTRUCTED_WALL :
            tileNode->addChild(getTerrain("tiles/rect_gray", 3, 40.0f, x, y));
            break;
          case WINDOW :
            tileNode->addChild(getTerrain("tiles/window", 0, 10.0f, x, y));
            break;
        }
        ground->addChild(floorNode);
        obj->addChild(tileNode);
      }
    }
    //level->addChild(ground);
    level->addChild(obj);
    return level;
  }

  auto materializePlayer(TileMap& map) -> std::shared_ptr<Node<Transform3D>> {
    auto tile = map.getRandom(PLAYER_SPAWN_POINT, _rand);
    auto node = std::make_shared<Node<Transform3D>>("player");
    node->setLocalPosition(glm::vec3(tile.getX(), tile.getY(), 2));
    node->addAttachment(getSprite("tiles/spriggan_druid", -1));
    auto physCircle = createPhysCircle(tile.getX(), tile.getY());
    node->addBehavior<PlayerBehaviour>(physCircle);
    return node;
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
  auto getTerrain(std::string sprites, int spriteVar, float health, int x, int y) -> std::shared_ptr<Node<Transform3D>> {
    auto node = std::make_shared<Node<Transform3D>>(name("obj",x,y));
    node->setLocalPosition(glm::vec3(x, y, 0));
    node->addAttachment(getSprite(sprites, spriteVar));
    auto physBody = createPhysSquare(x, y);
    node->addBehavior<TerrainBehaviour>(health, physBody);
    return node;
  }
};
