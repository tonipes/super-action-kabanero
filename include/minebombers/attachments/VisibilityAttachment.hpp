#pragma once

#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBVector.hpp"
#include "minebombers/level/TileMap.hpp"
#include "scene/NodeAttachment.hpp"

#include "service/Services.hpp"

class VisibilityAttachment : public NodeAttachment {
public:
  VisibilityAttachment(int width, int height, std::shared_ptr<TileMap> tilemap): _tilemap(tilemap) {
    Services::logger()->debug("Visibility create");

    for (auto x = 0; x < width; x++) {
      _visited.push_back(std::vector<bool>());
      _visible.push_back(std::vector<bool>());
      for (auto y = 0; y < height; y++) {
        _visited[x].push_back(false);
        _visible[x].push_back(false);
      }
    }
  }

  auto isVisible(int x, int y) -> bool {
    if(
      (x > -1 && x < _visible.size() ) &&
      (y > -1 && y < _visible[0].size() )
    ){
      return _visible[x][y];
    } else {
      return true;
    }
  }

  auto markVisited(int x, int y) const -> void;

private:
  mutable std::shared_ptr<TileMap> _tilemap;
  mutable std::vector<std::vector<bool>> _visited;
  mutable std::vector<std::vector<bool>> _visible;
};
