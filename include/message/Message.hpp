#pragma once

#include "message/Event.hpp"
#include <string>

/**
 * Message class.
 */
struct Message {
public:
  Message(std::string address, Event event) : event(event), address(address) {}

  const std::string address;
  const Event event;
};
