#pragma once

#include "game/Updateable.hpp"
#include "scene/Scene.hpp"
#include "scene/scene/GameScene.hpp"
#include "graphics/Renderer.hpp"
#include "message/MessageSubscriber.hpp"
#include "scene/2D/Transform2D.hpp"

/**
 * Game class.
 * @todo Resizing could also do resizing with message.
 * @todo Could be more generic. (transform)
 */
class Game: public Updateable, public MessageSubscriber  {
public:
  typedef Transform2D Transform;

  Game(Renderer& renderer): Updateable(), MessageSubscriber(), _renderer(renderer) {}
  ~Game() {}

  auto init() -> void override {}

  auto resize(int x, int y) -> void {}

  /**
   * Render game with resources.
   * @param delta time since last render.
   * @param resourceManager to get resources from.
   */
  auto render(double delta, ResourceManager& resourceManager) -> void {
    _renderer.render<Transform>(_scene, resourceManager);
  }

  auto update(double delta) -> void override {
    _scene.update(delta);
  }

  auto receiveMessage(Message& message) -> void override {}

private:
  Renderer _renderer;
  GameScene<Transform> _scene; // Placeholder. There will be multiple scenes in a game
};
