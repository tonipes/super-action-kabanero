#include "service/Services.hpp"

// Set messagePublisher default to NullMessagePublisher
std::shared_ptr<MessagePublisher> Services::_messagePublisher
  = std::make_shared<NullMessagePublisher>();
// Set resourceManager default to NullResourceManager
std::shared_ptr<ResourceManager> Services::_resourceManager
  = std::make_shared<NullResourceManager>();
// Set logger default to NullLogger
std::shared_ptr<Logger> Services::_logger
  = std::make_shared<NullLogger>();

std::shared_ptr<Random> Services::_random
  = std::make_shared<NullRandom>();
