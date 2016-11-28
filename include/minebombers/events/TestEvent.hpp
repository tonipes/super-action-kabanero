#pragma once

#include "message/Event.hpp"

enum TestAction {
  A,
  B
};

class TestEvent : public Event {
public:
  TestEvent(TestAction action) : _action(action) { }

  auto action() const -> TestAction {
    return _action;
  }
private:
  TestAction _action;
};
