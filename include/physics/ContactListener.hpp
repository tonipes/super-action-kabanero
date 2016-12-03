#pragma once

#include <Box2D/Box2D.h>
#include "service/Services.hpp"

class ContactListener : public b2ContactListener {
  void BeginContact(b2Contact* contact) {
    Services::logger()->debug("BeginContact");
  }

  void EndContact(b2Contact* contact) {
    Services::logger()->debug("EndContact");
  }
};
