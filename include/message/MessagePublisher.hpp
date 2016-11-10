#pragma once

#include "message/Message.hpp"

/**
 * MessagePublisher class.
 */
class MessagePublisher {
public:
  MessagePublisher() {}
  ~MessagePublisher() {}

  auto sendMessage(Message& message) -> void {};

};
