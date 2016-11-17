#pragma once

#include "message/event/AudioEvent.hpp"
#include "message/EventHandler.hpp"
#include "service/Services.hpp"
#include "exception/EngineException.hpp"
#include "resource/resource/Audio.hpp"


#include <string>
#include <iostream>
#include <SFML/Audio.hpp>

class AudioClip : public EventHandler {
public:
  AudioClip(const std::string& audioPath) {
    auto resourceManager = Services::resourceManager();
    auto audio = resourceManager->get<Audio>(audioPath);

    if (audio.isDefined()) {
      auto& audioBuffer = audio.get().getBuffer();
      _sound.setBuffer(audioBuffer);
    } else {
      throw EngineException("Audio: " + audioPath + " not found");
    }

    this->addEventResponse([&](AudioEvent audioEvent) {
      auto action = audioEvent.action();
      if (action == PLAY) {
        _sound.play();
      } else if (action == PAUSE) {
        _sound.pause();
      } else if (action == STOP) {
        _sound.stop();
      }
    });
  }

private:
  sf::Sound _sound;
};
