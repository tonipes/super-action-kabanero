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
    activeScenes.foreach([&](auto& scene){
      renderer.render(scene->getSceneViews());
    });
  }

  auto update(double delta) -> void override {
    activeScenes.foreach([&](auto& scene){
      scene->update(delta);
    });
    for(auto n : _toBeDestryed) {
      auto parent = n->parent();
      if(parent.isDefined()){
        n->physics()->GetWorld()->DestroyBody( n->physics() );
        parent.get().removeChild(n->name());
      }
    }
    // for(auto n : _toBeAdded){
    //   Services::logger()->debug("Parent name: " + std::get<0>(n)->name());
    //   Services::logger()->debug("Node name: " + std::get<1>(n)->name());
    //   std::get<0>(n)->addChild(std::get<1>(n));
    // }
    _toBeDestryed = KBVector<std::shared_ptr<Node<Transform3D>>>();
    // _toBeAdded = KBVector<std::tuple<std::shared_ptr<Node<Transform3D>>, std::shared_ptr<Node<Transform3D>>>>();
  }

  auto getEventHandler(const std::string& address) -> EventHandler& override {
    return *this;
  }

  auto getAllEventHandlers() -> KBVector<EventHandler> override {
    auto v = KBVector<EventHandler>();
    v += *this;
    return v;
  }

  auto addScene(std::shared_ptr<Scene<Transform3D>> scene) {
    activeScenes += scene;
    Services::messagePublisher()->addSubscriber(scene);
  }

protected:
  KBVector<std::shared_ptr<Node<Transform3D>>> _toBeDestryed;
  KBVector<std::tuple<std::shared_ptr<Node<Transform3D>>, std::shared_ptr<Node<Transform3D>>>> _toBeAdded;

  KBVector<std::shared_ptr<Scene<Transform3D>>> activeScenes;
};
