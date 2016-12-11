#pragma once

#include "message/Event.hpp"

class CreateNodeEvent: public Event {
public:
  CreateNodeEvent(
    std::string parentPath,
    std::shared_ptr<Node> node,
    std::shared_ptr<b2BodyDef> bodyDef,
    std::shared_ptr<b2FixtureDef> fixtureDef
    ):
  _parentPath(parentPath),
  _node(node),
  _bodyDef(bodyDef),
  _fixtureDef(fixtureDef) {}

  auto parentPath() const -> std::string {
    return _parentPath;
  }

  auto bodyDef() -> std::shared_ptr<b2BodyDef> {
    return _bodyDef;
  }

  auto fixtureDef() -> std::shared_ptr<b2FixtureDef> {
    return _fixtureDef;
  }

  auto node() const -> std::shared_ptr<Node> {
    return _node;
  }

private:
  std::string _parentPath;
  std::shared_ptr<b2BodyDef> _bodyDef;
  std::shared_ptr<b2FixtureDef> _fixtureDef;
  std::shared_ptr<Node> _node;
};
