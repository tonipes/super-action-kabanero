#pragma once

#include "minebombers/events/TestEvent.hpp"
#include "behavior/Behavior.hpp"
#include "scene/Node.hpp"
#include "service/Services.hpp"

class CameraBehavior : public Behavior {
public:
  template <typename T>
  CameraBehavior(Node<T>* node) {
    node->addEventReactor([&](TestEvent event) {
      auto action = event.action();
      if (action == A) {
        Services::logger()->debug("A");
      } else if (action == B) {
        Services::logger()->debug("B");
      }
    });
  }

  auto update(float delta) -> void override {
    
  }
};
