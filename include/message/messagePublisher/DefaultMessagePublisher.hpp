#pragma once

#include "service/MessagePublisher.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * Default implementation of message publisher
 */
class DefaultMessagePublisher: public MessagePublisher {
public:
  DefaultMessagePublisher() {}
  ~DefaultMessagePublisher() {}

  auto sendMessage(Message message) -> void override {
    _messages += message;
  }

  auto addSubscriber(MessageSubscriber& sub) -> void override {
    _subscribers += sub;
  }

  auto publishMessages() -> void override {
    for(auto& msg : _messages) {
      for(auto sub : _subscribers) {
        sub.get().receiveMessage(msg);
      }
    }
    _messages = KBVector<Message>();
  }

private:
  KBVector<std::reference_wrapper<MessageSubscriber>> _subscribers;
  KBVector<Message> _messages;
};
