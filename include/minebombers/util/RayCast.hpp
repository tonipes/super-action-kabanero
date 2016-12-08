#pragma once

#include "minebombers/util/Ray.hpp"
#include "minebombers/level/TileMap.hpp"
#include <cmath>

namespace RayCast {
  auto doRay(const std::shared_ptr<TileMap>& map, float startX, float startY, glm::vec2& dir, float stepSize, float maxLen) -> std::shared_ptr<Ray> {
    auto ray = std::make_shared<Ray>();
    auto remaining = maxLen;
    auto currentX = startX;
    auto currentY = startY;
    auto xStep = dir.x * stepSize;
    auto yStep = dir.y * stepSize;
    while (remaining >= 0) {
      auto currentTile = (*map)[floor(currentX)][floor(currentY)];
      if (!ray->containsTile(currentTile)) ray->registerTile(currentTile);
      if (!currentTile.isSeeThrough()) break;
      currentX += xStep;
      currentY += yStep;
      remaining -= stepSize;
    }
    return ray;
  }

  auto lineOfSight(const std::shared_ptr<TileMap>& map, float startX, float startY, int accuracy, float rayStep, float rayLen) -> KBVector<std::shared_ptr<Ray>> {
    auto angle = 0.0f;
    auto angle_increment = (float) (M_PI * 2 / accuracy);
    auto rays = KBVector<std::shared_ptr<Ray>>();
    while (angle < M_PI * 2) {
      auto vec = glm::vec2(cos(angle), sin(angle));
      auto ray = RayCast::doRay(map, startX, startY, vec, rayStep, rayLen);
      rays += ray;
      angle += angle_increment;
    }
    return rays;
  }

};
