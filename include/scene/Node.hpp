#pragma once

#include "kabanero/collection/mutable/KBVector.hpp"
#include <typeinfo>

template <typename T>
class Node {
public:
  Node(std::string name) : name(name) {}

  template <T>
  auto get() -> void {
    std::cout << typeid(T).name() << std::endl;
  }
  
private:
  std::string name;
  T transform;
  KBVector<Node<T>> children;
};
