#pragma once

#include "message/Event.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "util/LambdaUtil.hpp"
// #include "scene/Node.hpp"

#include <functional>
#include <memory>

template <typename T>
class Node;

/**
 * Base behavior class.
 */
template <typename T>
class Behavior {
public:
  virtual auto update(float delta, Node<T>& node) -> void { };
};
