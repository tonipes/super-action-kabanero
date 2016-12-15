#pragma once

#include "message/Event.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "util/LambdaUtil.hpp"

#include <functional>
#include <memory>

class Node;

class Behavior {
public:
  virtual auto update(float delta, std::shared_ptr<Node> node) -> void { };
};
