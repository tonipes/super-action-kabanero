#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * MessagePublisher class.
 * @todo Messages are now recieved immediately
 * @todo Use addesses. Now everybody gets every message.
 */
class MessagePublisher {
public:
  MessagePublisher() {}
  ~MessagePublisher() {}

  /**
   * Send message to subscribers.
   * @param message to send
   */
  auto sendMessage(Message message) -> void {
    for(auto sub : _subscribers) {
      sub.get().receiveMessage(message);
    }
  };

  /**
   * Add subscriber.
   * @param message to send
   */
  auto addSubscriber(MessageSubscriber& sub) -> void {
    _subscribers += sub;
  };

private:
  KBVector<std::reference_wrapper<MessageSubscriber>> _subscribers;
};
