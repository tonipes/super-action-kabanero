#pragma once

#include "service/MessagePublisher.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "util/StringUtil.hpp"
#include "exception/EngineException.hpp"
#include "service/Services.hpp"

#include <string>

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
      if (socketAndPath.length() == 0) {
        throw EngineException("No message address provided");
      }
      auto socket = socketAndPath[0];
      if (socket.compare("all") == 0) { // They match
        _subscribers.values().foreach([&](auto subscriber) {
          if (auto sharedPtr = subscriber.lock()) {
            sharedPtr->getAllEventHandlers().foreach([&](auto eventHandler) {
              eventHandler.handleEvent(message.event());
            });
          }
        });
      } else if (socketAndPath.length() == 1) {
        auto subscriber = _subscribers[socket];

        if (auto sharedPtr = subscriber.lock()) {
          auto& eventHandler = sharedPtr->getEventHandler("");
          eventHandler.handleEvent(message.event());
        }
      } else {
        auto path = socketAndPath[1];
        auto subscriber = _subscribers[socket];

        if (auto sharedPtr = subscriber.lock()) {
          auto& eventHandler = sharedPtr->getEventHandler(path);
          eventHandler.handleEvent(message.event());
        }
      }
    }
    _messages = KBVector<Message>();
  }

private:
  KBMap<std::string, std::weak_ptr<MessageSubscriber>> _subscribers;
  KBVector<Message> _messages;
};
