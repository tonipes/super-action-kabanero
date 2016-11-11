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

  auto init(
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void override {}

  auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void override {}

private:
  Node<T> _root;
};
