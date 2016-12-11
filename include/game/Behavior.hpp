#pragma once

#include "message/Event.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "util/LambdaUtil.hpp"
// #include "scene/Node.hpp"

#include <functional>
#include <memory>

class Node;

class Behavior {
public:
  virtual auto update(float delta, Node& node) -> void { };
};
