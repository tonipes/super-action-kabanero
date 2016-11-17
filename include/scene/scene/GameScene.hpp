#pragma once

#include "scene/Scene.hpp"
#include "message/EventHandler.hpp"

/**
 * Scene interface.
 */
template <typename T>
class GameScene : public Scene<T> {
public:
  GameScene(std::string name) : Scene<T>(name) {}
  virtual ~GameScene() {}

  auto init() -> void override {
    // std::cout << "GameScene init" << std::endl;
  }

  auto update(double delta) -> void override {
    // std::cout << "GameScene update with delta of " << delta << std::endl;
  }

  auto getEventHandler(const std::string& address) const -> EventHandler& override {
    auto eh = EventHandler();
    return eh;
  }
};
