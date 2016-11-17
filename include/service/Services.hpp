#pragma once

#include "service/MessagePublisher.hpp"
#include "service/ResourceManager.hpp"

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
   * Get resource manager
   * @return supplied resource manager
   */
  static auto resourceManager() -> std::shared_ptr<ResourceManager> {
    return _resourceManager;
  }

  /**
   * Replace current message publisher
   * @param messagePublisher new message publisher
   */
  static auto provideMessagePublisher(
      std::shared_ptr<MessagePublisher> messagePublisher) -> void {
    _messagePublisher = messagePublisher;
  }

  /**
   * Replace current resource manager
   * @param resourceManager new resource manager
   */
  static auto provideResourceManager(
      std::shared_ptr<ResourceManager> resourceManager) -> void {
    _resourceManager = resourceManager;
  }

private:
  static std::shared_ptr<MessagePublisher> _messagePublisher;
  static std::shared_ptr<ResourceManager> _resourceManager;
};

// Set messagePublisher default to NullMessagePublisher
std::shared_ptr<MessagePublisher> Services::_messagePublisher
  = std::make_shared<NullMessagePublisher>();
// Set resourceManager default to NullResourceManager
std::shared_ptr<ResourceManager> Services::_resourceManager
  = std::make_shared<NullResourceManager>();
