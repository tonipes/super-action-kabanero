#pragma once

#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class MatrixUtil {
public:
  static auto getTransform(glm::mat4x4 m) -> glm::vec3 {
    return glm::vec3(m[3]);
  }

  static auto getTransform(glm::mat3x3 m) -> glm::vec2 {
    return glm::vec2(m[2]);
  }
};
