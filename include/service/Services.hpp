#pragma once

#include "service/MessagePublisher.hpp"

#include <memory>

class Services {
public:
  /**
   * Get message publisher
   * @return supplied messagePublisher
   */
  static auto messagePublisher() -> std::shared_ptr<MessagePublisher> {
    return _messagePublisher;
  }

  /**
   * Replace current message publisher
   * @param  messagePublisher new message publisher
   */
  static auto provideMessagePublisher(
      std::shared_ptr<MessagePublisher> messagePublisher) -> void {
    _messagePublisher = messagePublisher;
  }

private:
  static std::shared_ptr<MessagePublisher> _messagePublisher;
};

// Set messagePublisher default to NullMessagePublisher
std::shared_ptr<MessagePublisher> Services::_messagePublisher
  = std::make_shared<NullMessagePublisher>();
