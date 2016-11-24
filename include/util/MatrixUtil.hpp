#pragma once

#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class MatrixUtil {
public:
  static auto getTransform(glm::mat4x4 m) -> glm::vec3 {
    return m[3].xyz;
  }

  static auto getTransform(glm::mat3x3 m) -> glm::vec2 {
    return m[2].xy;
  }
};
