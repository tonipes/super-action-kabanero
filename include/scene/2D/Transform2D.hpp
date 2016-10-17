#pragma once

#include "scene/Transform.hpp"
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

class Transform2D : public Transform<glm::vec2, float, glm::mat3x3> {
public:
  Transform2D() : Transform(glm::vec2(), 0.0f, glm::vec2(1, 1)) {}
  Transform2D(
      glm::vec2 pos,
      float rot,
      glm::vec2 scale
  ) : Transform(pos, rot, scale) {}

protected:
  void recomputeTransform() {
    _transform = glm::mat3x3();
    _transform = glm::scale(_transform, _scale);
    _transform = glm::rotate(_transform, _rotation);
    _transform = glm::translate(_transform, _position);
  }
};
