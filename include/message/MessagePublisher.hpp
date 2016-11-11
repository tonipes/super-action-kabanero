#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * MessagePublisher class.
 */
class MessagePublisher {
public:
  MessagePublisher() {}
  ~MessagePublisher() {}

  /**
   * Send message to subscribers.
   * @param message to send
   */
  auto sendMessage(Message& message) -> void {};

private:
  KBVector<MessageSubscriber> _subscribers;
};
