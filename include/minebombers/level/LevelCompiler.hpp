#pragma once

#include "minebombers/level/TileMap.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include <sstream>

class LevelCompiler {
public:
  LevelCompiler(Random& rand): _rand(rand) {}
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
        node->setLocalPosition(glm::vec3(x, y, 0));
        if (map[x][y].getType() == CAVE_WALL) {
          node->addAttachment(getSprite("tiles/pebble_brown", 8));
        } else if (map[x][y].getType() == INDESCTRUCTIBLE_WALL) {
          node->addAttachment(getSprite("tiles/stone_brick", 11));
        } else if (map[x][y].getType() == CONSTRUCTED_WALL) {
          node->addAttachment(getSprite("tiles/rect_gray", 3));
        } else if (map[x][y].getType() == WINDOW) {
          node->addAttachment(getSprite("tiles/window", 0));
        }
        objects->addChild(node);
      }
    }
    return objects;
  }
private:
  Random& _rand;
  auto getSprite(std::string baseName, int variations) -> std::shared_ptr<SpriteAttachment> {
      std::ostringstream oss;
      oss << baseName << _rand.nextInt(variations);
      return std::make_shared<SpriteAttachment>(oss.str());
  }
};
