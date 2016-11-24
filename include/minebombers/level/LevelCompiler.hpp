#pragma once

#include "minebombers/level/TileMap.hpp"

class LevelCompiler {
public:
  LevelCompiler(int tileSize) : _tileSize(tileSize) {}
  auto materialize(TileMap& map, std::shared_ptr<Node<Transform3D>> rootNode) -> void {
    auto ground = std::make_shared<Node<Transform3D>>("ground");
    auto objects = std::make_shared<Node<Transform3D>>("objects");
    for (auto x = 0; x < map.getWidth(); x++) {
      for (auto y = 0; y < map.getHeight(); y++) {
        auto node = std::make_shared<Node<Transform3D>>("ground-" + x + "/" + y);
        node->addAttachment(std::make_shared<SpriteAttachment>("tiles/dirt0"))
        node->setLocalPosition(glm::vec3(x, y, 0));
      }
    }
  }
private:
  int _tileSize;
}
