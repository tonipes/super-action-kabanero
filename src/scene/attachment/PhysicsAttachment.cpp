#include "scene/attachment/PhysicsAttachment.hpp"
//
// PhysicsAttachment::PhysicsAttachment(b2Body* body) : _body(body) { }
//
// auto PhysicsAttachment::position() -> glm::vec2 {
//   auto pos = _body->GetPosition();
//   return glm::vec2(pos.x, pos.y);
// }
//
// auto PhysicsAttachment::velocity() const -> glm::vec2 {
//   auto pos = _body->GetLinearVelocity();
//   return glm::vec2(pos.x, pos.y);
// }
//
// auto PhysicsAttachment::setVelocity(float x, float y) -> void {
//   _body->SetLinearVelocity(b2Vec2(x, y));
// }
//
auto PhysicsAttachment::setPosition(float x, float y) -> void {
  _body->SetTransform(b2Vec2(x, y), _body->GetAngle());
}
//
// auto PhysicsAttachment::destroy() -> void {
//   delete (CollisionData*) _body->GetUserData();
//
//   _body->GetWorld()->DestroyBody( _body );
// }
//
// auto PhysicsAttachment::body() const -> b2Body* {
//   return _body;
// }
