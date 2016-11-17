#pragma once

#include "service/MessagePublisher.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "util/StringUtil.hpp"
#include "exception/EngineException.hpp"

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

  auto addSubscriber(const std::shared_ptr<MessageSubscriber>& subscriber) -> void override {
    _subscribers[subscriber->socket()] = subscriber;
  }

  auto publishMessages() -> void override {
    for (auto& message : _messages) {
      auto socketAndPath = split(message.address(), ':');
      if (socketAndPath.length() != 2) {
        throw EngineException("Invalid message address: " + message.address());
      }
      auto socket = socketAndPath[0];
      auto path = socketAndPath[1];
      auto subscriber = _subscribers[socket];

      if (auto sharedPtr = subscriber.lock()) {
        auto& eventHandler = sharedPtr->getEventHandler(path);
        eventHandler.handleEvent(message.event());
      }

    }
    _messages = KBVector<Message>();
  }

private:
  KBMap<std::string, std::weak_ptr<MessageSubscriber>> _subscribers;
  KBVector<Message> _messages;
};
