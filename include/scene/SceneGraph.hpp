#pragma once

#include "game/Updateable.hpp"
#include "scene/Node.hpp"

/**
 * SceneGraph class.
 */
template <typename T>
class SceneGraph : public Updateable {
public:
  SceneGraph(): _root("root") {}
  ~SceneGraph() {}

  auto init() -> void override {}

  auto update(double delta) -> void override {}

private:
  Node<T> _root;
};
