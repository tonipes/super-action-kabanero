#pragma once

#include <SFML/Audio.hpp>

#include "game/Updateable.hpp"
#include "resource/resource/Audio.hpp"
#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "service/Services.hpp"

/**
 * AudioPlayer class.
 */
class AudioPlayer: public Updateable, public MessageSubscriber {
public:
  AudioPlayer() : MessageSubscriber("audioPlayer") {}
  ~AudioPlayer() {std::cout << "~AudioPlayer" << std::endl;}

  auto init() -> void override {
    std::cout << "AudioPlayer init" << std::endl;

    auto resourceManager = Services::resourceManager();
    auto &a = resourceManager->get<Audio>("resources/audio/local_forecast.ogg").get().getBuffer();
    _music.setBuffer(a);

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
  AudioPlayer(AudioPlayer& audioPlayer) : MessageSubscriber(audioPlayer.socket()){}
  bool isPlaying = false;
  sf::Sound _music;
};
