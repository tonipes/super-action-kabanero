#pragma once

#include "message/Event.hpp"

class CreateNodeEvent: public Event {
public:
  CreateNodeEvent(
    std::string parentPath,
    // b2FixtureDef fixtureDef,
    std::shared_ptr<b2BodyDef> bodyDef,
    std::shared_ptr<b2Shape> shape,
    std::shared_ptr<Node<Transform3D>> node):
  _parentPath(parentPath),
  _shape(shape),
  _bodyDef(bodyDef),
  _node(node) {}

  auto parentPath() const -> std::string {
    return _parentPath;
  }

  auto bodyDef() ->std::shared_ptr<b2BodyDef> {
    return _bodyDef;
  }

  auto shape() ->std::shared_ptr<b2Shape> {
    return _shape;
  }

  auto node() const -> std::shared_ptr<Node<Transform3D>> {
    return _node;
  }

private:
  std::string _parentPath;
  std::shared_ptr<b2BodyDef> _bodyDef;
  std::shared_ptr<b2Shape> _shape;
  std::shared_ptr<Node<Transform3D>> _node;
};
