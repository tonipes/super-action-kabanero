#include "minebombers/attachments/VisibilityAttachment.hpp"
#include "minebombers/util/RayCast.hpp"

auto VisibilityAttachment::markVisited(int x, int y) const -> void {
    _visited[x][y] = true;
    _visible[x][y] = true;

    auto los = RayCast::lineOfSight(_tilemap, x + 0.5f, y + 0.5f, 50, 0.04f, 8.0f);
    for (auto ray : los) {
      for (auto t : ray->getTiles()) {
        _visible[t.getX()][t.getY()] = true;
      }
    }
}
