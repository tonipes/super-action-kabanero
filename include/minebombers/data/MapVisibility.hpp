#pragma once

#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBVector.hpp"

class MapVisibility {
public:
  MapVisibility(int width, int height) {
    for (auto x = 0; x < width; x++) {
      _visited += KBVector<bool>();
      for (auto y = 0; y < height; y++) {
        _visited[x] += false;
      }
    }
  }

  auto isVisible(int x, int y) -> bool {
    return _visible[x][y];
  }
  
  auto isVisited(int x, int y) -> bool {
    return _visited[x][y];
  }

  auto markVisited(int x, int y) -> void {
    if(!_visited[x][y]){
      // Unvisited. Calculate new visibility
      _visited[x][y] = true;
    }
  }
private:
  KBVector<KBVector<bool>> _visited;
  KBVector<KBVector<bool>> _visible;
};
