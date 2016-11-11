#pragma once

#include <SFML/Audio.hpp>

#include "game/Updateable.hpp"
#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"

/**
 * AudioPlayer class.
 */
class AudioPlayer: public Updateable, public MessageSubscriber {
public:
  AudioPlayer() {}
  ~AudioPlayer() {}

  auto init(
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void override {
    std::cout << "AudioPlayer init" << std::endl;
  }

  auto update(
    double delta,
    MessagePublisher& messagePublisher,
    ResourceManager& resourceManager
  ) -> void override {
    std::cout << "AudioPlayer update with delta of " << delta << std::endl;
  }

  auto receiveMessage(Message& message) -> void override {
   std::cout << "AudioPlayer received a message " << std::endl;
  }

private:
  sf::SoundBuffer buffer;
};
