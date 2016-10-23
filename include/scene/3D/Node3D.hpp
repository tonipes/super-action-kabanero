#pragma once

#include "scene/3D/Transform3D.hpp"
#include "scene/Node.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Node3D : public Node<Transform3D> {
public:
  Node3D(std::string name) : Node(name) {}

  auto right() -> glm::vec3 {
    return worldTransform()[0].xyz();
  }

  auto up() -> glm::vec3 {
    return worldTransform()[1].xyz();
  }

  auto forward() -> glm::vec3 {
    return worldTransform()[2].xyz();
  }
};
