#pragma once

#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "message/MessagePublisher.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * DefaultMessagePublisher class.
 */
class DefaultMessagePublisher: public MessagePublisher {
public:
  DefaultMessagePublisher() {}
  ~DefaultMessagePublisher() {}

  /**
   * Send message to subscribers.
   * @param message to send
   */
  auto sendMessage(Message message) -> void override {
    for(auto sub : _subscribers) {
      sub.get().receiveMessage(message);
    }
  };

  /**
   * Add subscriber.
   * @param message to send
   */
  auto addSubscriber(MessageSubscriber& sub) -> void override {
    _subscribers += sub;
  };

private:
  KBVector<std::reference_wrapper<MessageSubscriber>> _subscribers;
};
