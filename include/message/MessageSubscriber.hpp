#pragma once

#include "message/Message.hpp"
#include <string>

/**
 * MessageSubscriber class.
 */
class MessageSubscriber {
public:
  MessageSubscriber(std::string socket) : _socket(socket) {}
  virtual ~MessageSubscriber() {}

  /**
   * Receive a Message.
   * @param message to receive.
   */
  virtual auto receiveMessage(Message& message) -> void = 0;

  auto socket() const -> std::string {
    return _socket;
  }

private:
  std::string _socket;
};
