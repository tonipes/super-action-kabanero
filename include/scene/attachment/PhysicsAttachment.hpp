#pragma once

#include "scene/NodeAttachment.hpp"
#include "physics/CollisionData.hpp"
#include <Box2D/Box2D.h>
#include <glm/vec2.hpp>

class PhysicsAttachment : public NodeAttachment {
public:

  PhysicsAttachment(b2Body* body) : _body(body) { }

  auto position() -> glm::vec2 {
    auto pos = _body->GetPosition();
    return glm::vec2(pos.x, pos.y);
  }

  auto velocity() const -> glm::vec2 {
    auto pos = _body->GetLinearVelocity();
    return glm::vec2(pos.x, pos.y);
  }

  auto setVelocity(float x, float y) -> void {
    _body->SetLinearVelocity(b2Vec2(x, y));
  }

  auto setPosition(float x, float y) -> void;

  auto destroy() -> void {
    delete (CollisionData*) _body->GetUserData();

    _body->GetWorld()->DestroyBody( _body );
  }

  auto body() const -> b2Body* {
    return _body;
  }

private:
  b2Body* _body;
};
