#pragma once

#include "game/Updateable.hpp"
#include "scene/Scene.hpp"
#include "scene/scene/GameScene.hpp"
#include "graphics/Renderer.hpp"
#include "message/MessageSubscriber.hpp"
#include "scene/2D/Transform2D.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

/**
 * Game class.
 * @todo Resizing could also do resizing with message.
 * @todo Could be more generic. (transform)
 */
class Game: public Updateable, public MessageSubscriber  {
public:
  typedef Transform2D Transform;

  Game() : Updateable(), MessageSubscriber("game") {}
  ~Game() {}

  virtual auto init() -> void = 0;

  auto resize(int x, int y) -> void {}

  /**
   * Render game with resources.
   * @param delta time since last render.
   * @param resourceManager to get resources from.
   */
  auto render(Renderer& renderer) -> void {
    activeScenes.foreach([&](auto& scene){
      renderer.render(scene->getSceneViews());
    });
  }

  auto update(double delta) -> void override {
    activeScenes.foreach([&](auto& scene){
      scene->update(delta);
    });
  }

  auto getEventHandler(const std::string& address) const -> EventHandler& override {
    auto eh = EventHandler();
    return eh;
  }

  auto addScene(std::shared_ptr<Scene<Transform3D>> scene) {
    activeScenes += scene;
    Services::messagePublisher()->addSubscriber(scene);
  }

private:
  KBVector<std::shared_ptr<Scene<Transform3D>>> activeScenes;
};
