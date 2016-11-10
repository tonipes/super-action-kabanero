#pragma once

#include "game/Updateable.hpp"

class Scene : public Updateable {
public:
  Scene() {}
  ~Scene() {}

  auto init(
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void {}

  auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void {
    std::cout << "Scene update with delta of " << delta << std::endl;
  }

};
