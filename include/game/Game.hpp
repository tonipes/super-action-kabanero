#pragma once

#include "game/Updateable.hpp"
#include "scene/Scene.hpp"
#include "graphics/Renderer.hpp"
#include "message/MessageSubscriber.hpp"

/**
 * Game class.
 */
class Game: public Updateable, public MessageSubscriber  {
public:
  Game(Renderer& renderer): Updateable(), MessageSubscriber(), _renderer(renderer) {}
  ~Game() {}

  auto init(
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void {}

  // TODO: Could also do resizing with message.
  auto resize(int x, int y) -> void {}

  /**
   * Render game with resources.
   * @param delta time since last render.
   * @param resourceManager to get resources from.
   */
  auto render(double delta, ResourceManager& resourceManager) -> void {
    _renderer.render(_scene, resourceManager);
  }

  auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void {
    _scene.update(delta, messagePublisher, resourceManager);
  }
private:
  Renderer _renderer;
  Scene _scene; // Placeholder. There will be multiple scenes in a game
};
