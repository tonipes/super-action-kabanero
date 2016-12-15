#pragma once

#include "message/Event.hpp"

enum TestAction {
  A,
  B
};

/**
 * An event used in internal testing. Currently ununsed.
 * Serves as an example of how an event should be structured.
 */
class TestEvent : public Event {
public:
  TestEvent(TestAction action) : _action(action) { }

  auto action() const -> TestAction {
    return _action;
  }
private:
  TestAction _action;
};
