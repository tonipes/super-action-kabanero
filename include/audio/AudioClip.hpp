#pragma once

#include "message/event/AudioClipEvent.hpp"
#include "message/EventHandler.hpp"
#include "service/Services.hpp"
#include "exception/EngineException.hpp"
#include "resource/resource/Audio.hpp"


#include <string>
#include <iostream>
#include <SFML/Audio.hpp>

class AudioClip : public EventHandler {
public:
  ~AudioClip() {
    _sound.stop();
  }
  AudioClip(const std::string& audioPath) {
    auto resourceManager = Services::resourceManager();
    auto audio = resourceManager->get<Audio>(audioPath);

    if (audio.isDefined()) {
      auto& audioBuffer = audio.get().getBuffer();
      _sound.setBuffer(audioBuffer);
    } else {
      throw EngineException("Audio: " + audioPath + " not found");
    }

    this->addEventReactor([&](AudioClipEvent audioEvent) {
      auto action = audioEvent.action();
      if (action == CLIP_PLAY) {
        _sound.play();
      }
    });
  }

  auto stop() -> void {
    _sound.stop();
  }

private:
  sf::Sound _sound;
};
