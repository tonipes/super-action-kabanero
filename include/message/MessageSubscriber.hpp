#pragma once

#include "message/Message.hpp"
#include "message/EventHandler.hpp"
#include <string>

/**
 * MessageSubscriber class.
 */
class MessageSubscriber {
public:
  MessageSubscriber(std::string socket) : _socket(socket) {}
  virtual ~MessageSubscriber() {}

  /**
   * Gets the event handler corresponding to the supplied path.
   * @param  path The path to the event handler, formatted "foo/bar/baz/..."
   * @return      Event handler
   */
  virtual auto getEventHandler(const std::string& path) -> EventHandler& = 0;

  virtual auto getAllEventHandlers() -> KBVector<EventHandler> = 0;

  auto socket() const -> std::string {
    return _socket;
  }

private:
  std::string _socket;
};
