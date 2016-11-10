#pragma once

#include "game/Updateable.hpp"

class SceneGraph : public Updateable {
public:
  SceneGraph() {}
  ~SceneGraph() {}

  auto init() -> void {}

  auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void {

  }

private:
  Node root;
};
