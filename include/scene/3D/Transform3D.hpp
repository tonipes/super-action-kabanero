#pragma once

#include "scene/Transform.hpp"
#include "util/PrintUtil.hpp"
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform3D : public Transform<glm::vec3, glm::quat, glm::mat4x4> {
public:
  Transform3D() : Transform(glm::vec3(0, 0, 0), glm::quat(), glm::vec3(1, 1, 1)) {}
  Transform3D(
      glm::vec3 pos,
      glm::quat rot,
      glm::vec3 scale
  ) : Transform(pos, rot, scale) {}

protected:
  void recomputeTransform() {
    _transform =
      glm::translate(_position) *
      glm::toMat4(_rotation) *
      glm::scale(_scale);
  }
};


std::ostream& operator<<(std::ostream& os, Transform3D t) {
  os << "Transform3D" << std::endl <<
    "  position: " << t.position() << std::endl <<
    "  rotation: " << t.rotation() << std::endl <<
    "  scale:  " << t.scale();

  return os;
}
