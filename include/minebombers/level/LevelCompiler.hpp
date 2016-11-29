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
  auto materializeGround(TileMap& map) -> std::shared_ptr<Node<Transform3D>> {
    auto ground = std::make_shared<Node<Transform3D>>("ground");
    for (auto x = 0; x < map.getWidth(); x++) {
      for (auto y = 0; y < map.getHeight(); y++) {
        std::ostringstream oss;
        oss << "ground" << x << "-" << y;
        auto node = std::make_shared<Node<Transform3D>>(oss.str());
        node->addAttachment(getSprite("tiles/dirt", 2));
        node->setLocalPosition(glm::vec3(x, y, 0));
        ground->addChild(node);
      }
    }
    return ground;
  }
  auto materializeObjects(TileMap& map) -> std::shared_ptr<Node<Transform3D>> {
    auto objects = std::make_shared<Node<Transform3D>>("objects");
    for (auto x = 0; x < map.getWidth(); x++) {
      for (auto y = 0; y < map.getHeight(); y++) {
        std::ostringstream oss;
        oss << "wall" << x << "-" << y;
        auto node = std::make_shared<Node<Transform3D>>(oss.str());
        node->setLocalPosition(glm::vec3(x, y, 1));
        auto physBody = createPhysSquare(x, y);
        auto health = 100.0f;
        if (map[x][y].getType() == CAVE_WALL) {
          node->addAttachment(getSprite("tiles/pebble_brown", 8));
        } else if (map[x][y].getType() == INDESCTRUCTIBLE_WALL) {
          health = 10000000.0f;
          node->addAttachment(getSprite("tiles/stone_brick", 11));
        } else if (map[x][y].getType() == CONSTRUCTED_WALL) {
          health = 40.0f;
          node->addAttachment(getSprite("tiles/rect_gray", 3));
        } else if (map[x][y].getType() == WINDOW) {
          health = 10.0f;
          node->addAttachment(getSprite("tiles/window", 0));
        }
        node->addBehavior<TerrainBehaviour>(health, physBody);
        objects->addChild(node);
      }
    }
    return objects;
  }

  auto materializePlayer(TileMap& map)  -> std::shared_ptr<Node<Transform3D>> {
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
    box.SetAsBox(1.0f, 1.0f);
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
};
