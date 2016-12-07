#pragma once

#include <string>
#include <iostream>
#include <Box2D/Box2D.h>
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "physics/CollisionData.hpp"

class ContactListener : public b2ContactListener {
  void BeginContact(b2Contact* contact) {
    CollisionData* a_data = NULL;
    CollisionData* b_data = NULL;

    void* a_userData = contact->GetFixtureA()->GetBody()->GetUserData();
    void* b_userData = contact->GetFixtureB()->GetBody()->GetUserData();

    bool a_isSensor = contact->GetFixtureA()->IsSensor();
    bool b_isSensor = contact->GetFixtureA()->IsSensor();

    if (a_userData) {
      a_data = (CollisionData*) a_userData;
    }
    if (b_userData) {
      b_data = (CollisionData*) b_userData;
    }

    if (a_data && b_data){
      if(!a_data->path().empty()){
        Services::messagePublisher()->sendMessage(
          Message("gameScene:" + a_data->path(), std::make_shared<CollisionEvent>(
            BEGIN,
            b_data->path(),
            b_data->collisionMaterialAttachment(),
            b_isSensor
          ))
        );
      }

      if(!b_data->path().empty()){
      Services::messagePublisher()->sendMessage(
        Message("gameScene:" + b_data->path(), std::make_shared<CollisionEvent>(
          BEGIN,
          a_data->path(),
          a_data->collisionMaterialAttachment(),
          a_isSensor
        ))
      );
    }}
  }

  void EndContact(b2Contact* contact) {

  }

};
