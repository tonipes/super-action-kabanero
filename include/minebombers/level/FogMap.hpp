#pragma once

#include <sstream>
#include "collection/mutable/KBMap.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include <sstream>

class FogVisibility {
public:
  FogVisibility(): visible(false) {}
  bool visible;
};

class FogMap {
public:
  FogMap() {}

  auto operator[](int x) -> KBVector<std::shared_ptr<Node>>& {
    return _map[x];
  }

  auto init(int width, int height) -> void {
    _map = KBVector<KBVector<std::shared_ptr<Node>>>();
    _visited = KBVector<KBVector<FogVisibility>>();
    for (auto x = 0; x < width; x++) {
      _map += KBVector<std::shared_ptr<Node>>();
      _visited += KBVector<FogVisibility>();
      for (auto y = 0; y < height; y++) {
        std::ostringstream oss;
        oss << "fog" << x << "-" << y;
        _map[x] += std::make_shared<Node>(oss.str());
        auto b = FogVisibility();
        _visited[x] += b;
      }
    }
  }

  auto isVisited(int x, int y) -> bool {
    return _visited[x][y].visible;
  }

  auto markVisited(int x, int y) -> void {
    _visited[x][y].visible = true;
  }
private:
  KBVector<KBVector<std::shared_ptr<Node>>> _map;
  KBVector<KBVector<FogVisibility>> _visited;
};
