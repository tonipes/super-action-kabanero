#pragma once

#include "scene/Scene.hpp"
#include "message/EventHandler.hpp"
#include "util/StringUtil.hpp"


/**
 * Scene interface.
 */
template <typename T>
class GameScene : public Scene<T>, public EventHandler {
public:
  GameScene(std::string name, std::shared_ptr<Node<T>> rootNode) : Scene<T>(name, rootNode), _physWorld(b2Vec2(0.0f,0.0f)) {}
  virtual ~GameScene() {}

  auto init() -> void override {
    // std::cout << "GameScene init" << std::endl;
  }

  auto update(double delta) -> void override {
    _phys_elapsed += delta;
    while (_phys_elapsed >= _phys_step) {
      _phys_elapsed -= _phys_step;
      _physWorld.Step(_phys_step, 8, 3);
    }
    this->rootNode()->update(delta);
  }

  auto getEventHandler(const std::string& address) -> EventHandler& override {
    auto names = split(address, '/');
    auto node = this->rootNode();
    auto notFound = false;
    for (auto i = 0; i < names.length(); i++) {
      auto n = node->children().get(names[i]);
      if (n.isDefined()) {
        node = n.get();
      } else {
        notFound = true;
      }
    }
    if (notFound) {
      throw EngineException("No node with address: [" + address + "] found");
    }

    return *node;
  }

  auto getAllEventHandlers() -> KBVector<EventHandler> override {
    auto nodes = _getAllNodes(this->rootNode());
    auto v = KBVector<EventHandler>();
    nodes.foreach([&](auto ptr) {
      v += *ptr;
    });
    return v;
  }
private:
  b2World _physWorld;
  double _phys_elapsed = 0.0;
  const double _phys_step = 1000.0 / 30.0;
  auto _getAllNodes(std::shared_ptr<Node<T>> node) -> KBVector<std::shared_ptr<Node<T>>> {
    auto v = KBVector<std::shared_ptr<Node<T>>>();
    v += node;
    auto children = node->children().values();
    children.foreach([&](auto child) {
      auto n = _getAllNodes(child);
      for (auto c : n) {
        v += c;
      }
    });
    return v;
  }
};
