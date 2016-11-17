#pragma once

#include "message/Event.hpp"
#include <string>

/**
 * Message class.
 */
class Message {
public:
  Message(std::string address, std::shared_ptr<Event> event) : _event(event), _address(address) {}

  auto address() -> std::string {
    return _address;
  }

  auto event() -> std::shared_ptr<Event> {
    return _event;
  }

private:
  std::string _address;
  std::shared_ptr<Event> _event;
};
