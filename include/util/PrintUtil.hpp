#pragma once

#include <iostream>
#include <tuple>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

std::ostream& operator<<(std::ostream& os, glm::vec3 v);

std::ostream& operator<<(std::ostream& os, glm::quat v);

std::ostream& operator<<(std::ostream& os, glm::mat4x4 m);


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
