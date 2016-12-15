#include "graphics/Renderer.hpp"

auto Renderer::_isWithinWindow(const glm::vec3& relativePosition, const BoundingBox& boundingBox) -> bool {
  float leftSide = relativePosition.x + boundingBox.left();
  float rightSide = relativePosition.x + boundingBox.right();
  float topSide = relativePosition.y + boundingBox.top();
  float bottomSide = relativePosition.y + boundingBox.bottom();

  return
    leftSide < rightEdge && rightSide > leftEdge &&
    topSide > bottomEdge && bottomSide < topEdge;
}
