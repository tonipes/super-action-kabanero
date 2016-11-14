#pragma once

#include "scene/Scene.hpp"

/**
 * Scene interface.
 */
template <typename T>
class GameScene : public Scene<T> {
public:
  GameScene() {}
  virtual ~GameScene() {}

  auto init(
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void override {
    // std::cout << "GameScene init" << std::endl;
  }

  auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void override {
    // std::cout << "GameScene update with delta of " << delta << std::endl;
  }

  auto receiveMessage(Message& message) -> void override {
  //  std::cout << "GameScene received a message " << std::endl;
  }
};
