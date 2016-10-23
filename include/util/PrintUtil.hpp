#pragma once

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

std::ostream& operator<<(std::ostream& os, glm::vec3 v) {
  os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, glm::quat v) {
  os << "quat(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, glm::mat4x4 m) {
  os << "mat4x4" << std::endl <<
    "  [" << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << "]" << std::endl <<
    "  [" << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << "]" << std::endl <<
    "  [" << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << "]" << std::endl <<
    "  [" << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << "]";
  return os;
}


template<std::size_t> struct int_{};

template <class Tuple, size_t Pos>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<Pos> ) {
  out << std::get< std::tuple_size<Tuple>::value-Pos >(t) << ", ";
  return print_tuple(out, t, int_<Pos-1>());
}

template <class Tuple>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<1> ) {
  return out << std::get<std::tuple_size<Tuple>::value-1>(t);
}

template <class... Args>
std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& t) {
  out << '(';
  print_tuple(out, t, int_<sizeof...(Args)>());
  return out << ')';
}
