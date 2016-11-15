#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * MessagePublisher interface.
 */
class MessagePublisher {
public:
  MessagePublisher() {}
  virtual ~MessagePublisher() {}

  /**
   * Send message to subscribers.
   * @param message to send
   */
  virtual auto sendMessage(Message message) -> void = 0;

  /**
   * Add subscriber.
   * @param message to send
   */
  virtual auto addSubscriber(MessageSubscriber& sub) -> void = 0;

  /**
   * Publishes messages.
   */
  virtual auto publishMessages() -> void = 0;

private:
  MessagePublisher(const MessagePublisher& messagePublisher) {}

};
