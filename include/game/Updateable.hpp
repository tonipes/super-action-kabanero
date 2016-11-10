#pragma once

#include "message/MessagePublisher.hpp"
#include "resource/ResourceManager.hpp"

/**
 * Updateable class.
 */
class Updateable {
public:
  Updateable() {}
  ~Updateable() {}

  /**
   * Render game with resources.
   * @param messagePublisher MessagePublisher.
   * @param resourceManager ResourceManager.
   */
  virtual auto init(
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void = 0;

  /**
   * Update function.
   * @param delta time since last update.
   * @param messagePublisher MessagePublisher.
   * @param resourceManager ResourceManager.
   */
  virtual auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void = 0;
};
