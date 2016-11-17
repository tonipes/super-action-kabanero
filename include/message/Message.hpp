#pragma once

#include "message/Event.hpp"
#include <string>

/**
 * Message class.
 */
class Message {
public:
  Message(std::string address, Event event) : event(event), address(address) {}
  auto operator=(Message& msg) -> Message& {
    Message(msg.address, msg.event);
  }
  const std::string address;
  const Event event;
};
