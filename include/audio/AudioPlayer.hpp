#pragma once

#include <SFML/Audio.hpp>

#include "game/Updateable.hpp"
#include "resource/resource/Audio.hpp"
#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"

/**
 * AudioPlayer class.
 */
class AudioPlayer: public Updateable, public MessageSubscriber {
public:
  AudioPlayer() {}
  ~AudioPlayer() {std::cout << "~AudioPlayer" << std::endl;}

  auto init() -> void override {
    std::cout << "AudioPlayer init" << std::endl;

    // auto &a = resourceManager.get<Audio>("resources/audio/local_forecast.ogg").getBuffer();
    // _music.setBuffer(a);

  }

  auto update(double delta) -> void override {
    // std::cout << "AudioPlayer update with delta of " << delta << std::endl;
  }

  auto receiveMessage(Message& message) -> void override {
    if(isPlaying)
      _music.pause();
    else
      _music.play();
    isPlaying = !isPlaying;

    std::cout << "AudioPlayer received a message " << std::endl;
  }

private:
  AudioPlayer(AudioPlayer& audioPlayer) {}
  bool isPlaying = false;
  sf::Sound _music;
};
