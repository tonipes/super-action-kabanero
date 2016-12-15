#pragma once

#include <string>
#include <iostream>
#include <Box2D/Box2D.h>
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "physics/CollisionData.hpp"
#include <glm/vec2.hpp>

class ContactListener : public b2ContactListener {
  void BeginContact(b2Contact* contact) {
    CollisionData* a_data = NULL;
    CollisionData* b_data = NULL;

    b2Body* a_body = contact->GetFixtureA()->GetBody();
    b2Body* b_body = contact->GetFixtureB()->GetBody();

    void* a_userData = a_body->GetUserData();
    void* b_userData = b_body->GetUserData();

    bool a_isSensor = contact->GetFixtureA()->IsSensor();
    bool b_isSensor = contact->GetFixtureA()->IsSensor();

    b2Manifold* manifold = contact->GetManifold();

    auto a_to_b = b_body->GetPosition() - a_body->GetPosition();
    auto b_to_a = a_body->GetPosition() - b_body->GetPosition();

    // Cast collision datas
    if (a_userData) {
      a_data = (CollisionData*) a_userData;
    }
    if (b_userData) {
      b_data = (CollisionData*) b_userData;
    }

    // Send collision events
    if (a_data && b_data){
      if(!a_data->path().empty()){
        Services::messagePublisher()->sendMessage(
          Message("gameScene:" + a_data->path(), std::make_shared<CollisionEvent>(
            BEGIN,
            b_data->path(),
            b_data->collisionMaterialAttachment(),
            b_isSensor,
            glm::vec2(b_to_a.x, b_to_a.y)
          ))
        );
      }

      if(!b_data->path().empty()){
        Services::messagePublisher()->sendMessage(
          Message("gameScene:" + b_data->path(), std::make_shared<CollisionEvent>(
            BEGIN,
            a_data->path(),
            a_data->collisionMaterialAttachment(),
            a_isSensor,
            glm::vec2(a_to_b.x, a_to_b.y)
          ))
        );
      }
    }

    // if(a_data->collisionMaterialAttachment()->force > 0.0f) {
    //   ApplyCollisionForce(a_body, b_body, a_data->collisionMaterialAttachment()->force);
    //
    // }
    // if(b_data->collisionMaterialAttachment()->force > 0.0f) {
    //   ApplyCollisionForce(b_body, a_body, b_data->collisionMaterialAttachment()->force);
    // }
  }

  void ApplyCollisionForce(b2Body* centerBody, b2Body* otherBody, float force) {
    b2Vec2 center_pos = centerBody->GetPosition();
    b2Vec2 body_pos = otherBody->GetPosition();
    b2Vec2 d = center_pos - body_pos;

    float f = force * 100.0f / (d.LengthSquared() + 1.0f);
    d.Normalize();
    b2Vec2 F = f * -d;

    otherBody->ApplyForce(F, body_pos, true);

  }

  void EndContact(b2Contact* contact) {

  }

};
