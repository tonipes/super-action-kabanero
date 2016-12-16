#pragma once

#include "scene/Scene.hpp"
#include "message/EventHandler.hpp"
#include "util/StringUtil.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "physics/ContactListener.hpp"
#include <Box2D/Box2D.h>

/**
 * Scene interface.
 */
class GameScene : public Scene<Transform3D>, public EventHandler {
public:
  GameScene(std::string name, std::shared_ptr<Node<Transform3D>> rootNode) :
      Scene<Transform3D>(name, rootNode), _physWorld(b2Vec2(0.0f,0.0f)), _phys_elapsed(0.0), _phys_step(0.033) {

    _physWorld.SetContactListener(&_contactListener);
    this->addEventReactor([&](DestroyNodeEvent event) {
      auto path = event.path();
      _destroyNode(path);
    });
    this->addEventReactor([&](CreateNodeEvent event) {
      auto path = event.parentPath();

      const auto& node = event.node();

      if (event.bodyDef()) {
        b2Body* body = _physWorld.CreateBody(event.bodyDef().get());
        body->CreateFixture(event.fixtureDef().get());

        delete event.fixtureDef().get()->shape;

        auto physAttachment = std::make_shared<PhysicsAttachment>(body);
        node->addAttachment(physAttachment);
      }

      _toBeAdded += std::make_tuple(path, node);
    });
  }
  virtual ~GameScene() {}

  auto init() -> void override {}

  auto update(double delta) -> void override {
    for (auto path : _toBeDestroyed) {
      const auto& n = getNode(path);
      auto parent = n->parent();
      if (parent.isDefined()) {
        const auto& physAttachment = n->get<PhysicsAttachment>();
        physAttachment.foreach([&](auto& phys) {
          phys.destroy();
        });
        parent.get().removeChild(n->name());
      }
    }
    _toBeDestroyed = KBVector<std::string>();
    for (auto n : _toBeAdded) {
      auto path = std::get<0>(n);
      auto& node = std::get<1>(n);

      const auto& parentNode = getNode(path);
      parentNode->addChild(node);
    }
    _toBeAdded = KBVector<std::tuple<std::string, std::shared_ptr<Node<Transform3D>>>>();
    _physWorld.Step(_phys_step, 8, 3);
    this->rootNode()->update(delta);
  }

  auto getEventHandler(const std::string& address) -> EventHandler& override {
    if (address.length() == 0) {
      return *this;
    } else {
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
        Services::logger()->error("No node with address: [" + address + "] found");
      }

      return *node;
    }
  }

  auto getNode(const std::string& address) -> std::shared_ptr<Node<Transform3D>> {
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
      Services::logger()->error("No node with address: [" + address + "] found");
    }

    return node;
  }

  auto physWorld() -> b2World& {
    return _physWorld;
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
  KBVector<std::string> _toBeDestroyed;
  KBVector<std::tuple<std::string, std::shared_ptr<Node<Transform3D>>>> _toBeAdded;
  b2World _physWorld;
  ContactListener _contactListener;
  double _phys_elapsed;
  const double _phys_step;

  auto _destroyNode(std::string path) -> void {
    _toBeDestroyed += path;
  }

  auto _getAllNodes(std::shared_ptr<Node<Transform3D>> node) -> KBVector<std::shared_ptr<Node<Transform3D>>> {
    auto v = KBVector<std::shared_ptr<Node<Transform3D>>>();
    v += node;
    auto children = node->children().values();
    children.foreach([&](auto child) {
      auto n = this->_getAllNodes(child);
      for (auto c : n) {
        v += c;
      }
    });
    return v;
  }
};
