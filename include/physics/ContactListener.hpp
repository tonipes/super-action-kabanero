#pragma once

#include <string>
#include <iostream>
#include <Box2D/Box2D.h>
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"

class ContactListener : public b2ContactListener {
  void BeginContact(b2Contact* contact) {
    std::string a_path = "";
    std::string b_path = "";

    void* a_userData = contact->GetFixtureA()->GetBody()->GetUserData();
    void* b_userData = contact->GetFixtureB()->GetBody()->GetUserData();

    if (a_userData){
      a_path = *(std::string*) a_userData;
      Services::messagePublisher()->sendMessage(Message("gameScene:" + a_path, std::make_shared<CollisionEvent>(TEST) ));

    }
    if (b_userData){
      b_path = *(std::string*) b_userData;
      Services::messagePublisher()->sendMessage(Message("gameScene:" + b_path, std::make_shared<CollisionEvent>(TEST) ));

    }

    Services::logger()->debug("BeginContact with " + a_path + " & " + b_path);

  }

  void EndContact(b2Contact* contact) {
    Services::logger()->debug("EndContact");
  }
};
