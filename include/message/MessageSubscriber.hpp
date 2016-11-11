#pragma once

#include "message/Message.hpp"

/**
 * MessageSubscriber class.
 * @todo Should socket be rather something else than a string?
 */
class MessageSubscriber {
public:
  MessageSubscriber() {}
  virtual ~MessageSubscriber() {}

  /**
   * Receive a Message.
   * @param message to receive.
   */
  virtual auto receiveMessage(Message& message) -> void {}

private:
  std::string _socket;
};
