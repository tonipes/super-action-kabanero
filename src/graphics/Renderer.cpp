#include "graphics/Renderer.hpp"

auto Renderer::_isWithinWindow(const glm::vec3& nodePosition, const BoundingBox& boundingBox) -> bool {
  const auto& relativePosition = (nodePosition - _cameraPosition) * (float)_tilesize;
  auto halfWidth = _viewportSize.x / 2.0f + _tilesize;
  auto halfHeight = _viewportSize.y / 2.0f + _tilesize;

  auto leftSide = relativePosition.x + boundingBox.left() * _tilesize;
  auto rightSide = relativePosition.x + boundingBox.right() * _tilesize;
  auto topSide = relativePosition.y + boundingBox.top() * _tilesize;
  auto bottomSide = relativePosition.y + boundingBox.bottom() * _tilesize;

  return
    leftSide < halfWidth && rightSide > -halfWidth &&
    topSide > -halfHeight && bottomSide < halfHeight;
}
