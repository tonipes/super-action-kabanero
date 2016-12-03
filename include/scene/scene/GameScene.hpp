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
  GameScene(std::string name, std::shared_ptr<Node<T>> rootNode) : Scene<T>(name, rootNode) {}
  virtual ~GameScene() {}

  auto init() -> void override {
    // std::cout << "GameScene init" << std::endl;
  }

  auto update(double delta) -> void override {
    this->rootNode()->update(delta);
  }

  auto getEventHandler(const std::string& address) -> EventHandler& override {
    auto names = split(address, '/');
    auto node = this->rootNode();
    auto notFound = false;
    for (auto i = 1; i < names.length(); i++) {
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
