#pragma once

#include "game/Updateable.hpp"
#include "scene/Scene.hpp"
#include "scene/scene/GameScene.hpp"
#include "graphics/Renderer.hpp"
#include "message/MessageSubscriber.hpp"
#include "scene/2D/Transform2D.hpp"
#include "scene/3D/Transform3D.hpp"

/**
 * Game class.
 * @todo Resizing could also do resizing with message.
 * @todo Could be more generic. (transform)
 */
class Game: public Updateable, public MessageSubscriber  {
public:
  typedef Transform2D Transform;

  Game(Renderer& renderer):
      Updateable(),
      MessageSubscriber("game"),
      _renderer(renderer),
      _scene(GameScene<Transform3D>("testScene")) {

  }
  ~Game() {}

  auto init() -> void override {}

  auto resize(int x, int y) -> void {}

  /**
   * Render game with resources.
   * @param delta time since last render.
   * @param resourceManager to get resources from.
   */
  auto render() -> void {
    _renderer.render<Transform3D>(_scene);
  }

  auto update(double delta) -> void override {
    _scene.update(delta);
  }

  auto receiveMessage(Message& message) -> void override {}

private:
  Renderer _renderer;
  GameScene<Transform3D> _scene; // Placeholder. There will be multiple scenes in a game
};
