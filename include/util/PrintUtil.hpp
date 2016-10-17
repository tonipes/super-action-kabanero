#pragma once

#include <iostream>
#include <glm/vec3.hpp>


std::ostream& operator<<(std::ostream& os, glm::vec3 v) {
  os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}
