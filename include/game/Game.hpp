#pragma once

#include <tuple>

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
class Game: public Updateable, public MessageSubscriber, public EventHandler {
public:
  typedef Transform2D Transform;

  Game() : Updateable(), MessageSubscriber("game") {}
  ~Game() {}

  virtual auto init() -> void override = 0;

  auto resize(int x, int y) -> void {}

  /**
   * Render game with resources.
   * @param delta time since last render.
   * @param resourceManager to get resources from.
   */
  auto render(Renderer& renderer) -> void {
    activeScene.foreach([&](auto& scene){
      renderer.render(scene->getSceneViews());
    });
  }

  auto update(double delta) -> void override {
    activeScene.foreach([&](auto& scene){
      scene->update(delta);
    });
  }

  auto getEventHandler(const std::string& address) -> EventHandler& override {
    return *this;
  }

  auto getAllEventHandlers() -> KBVector<EventHandler> override {
    auto v = KBVector<EventHandler>();
    v += *this;
    return v;
  }

  auto addScene(std::shared_ptr<Scene<Transform3D>> scene) -> void {
    // scenes = KBMap<std::string, std::shared_ptr<Scene<Transform3D>>>();
    auto name = scene->socket();
    scenes[name] = scene;
    Services::messagePublisher()->addSubscriber(scene);
  }

  auto activateScene(std::string name) {
    activeScene = Option<std::shared_ptr<Scene<Transform3D>>>(scenes[name]);
  }

protected:
  KBMap<std::string, std::shared_ptr<Scene<Transform3D>>> scenes;
  Option<std::shared_ptr<Scene<Transform3D>>> activeScene;
};
