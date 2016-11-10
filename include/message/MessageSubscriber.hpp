#pragma once

#include "message/Message.hpp"

/**
 * MessageSubscriber class.
 */
class MessageSubscriber {
public:
  MessageSubscriber() {}
  ~MessageSubscriber() {}

  /**
   * Receive a Message.
   * @param message to receive.
   */
  auto receiveMessage(Message& message) -> void {}
};
