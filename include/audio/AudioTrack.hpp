#pragma once

#include "message/event/AudioTrackEvent.hpp"
#include "message/EventHandler.hpp"
#include "service/Services.hpp"
#include "exception/EngineException.hpp"
#include "resource/resource/Audio.hpp"

#include <string>
#include <iostream>
#include <SFML/Audio.hpp>

class AudioTrack : public EventHandler {
public:
  AudioTrack() {
    _sound.setLoop(true);

    this->addEventReactor([&](AudioTrackEvent audioEvent) {
      auto action = audioEvent.action();
      if (action == TRACK_PLAY) {
        _sound.play();
      } else if (action == TRACK_PAUSE) {
        _sound.pause();
      } else if (action == TRACK_STOP) {
        _sound.stop();
      } else if (action == TRACK_CHANGE) {
        setBuffer(audioEvent.audioPath());
      }
    });
  }

private:
  sf::Sound _sound;

  auto setBuffer(const std::string& audioPath) -> void {
    auto resourceManager = Services::resourceManager();
    auto audio = resourceManager->get<Audio>(audioPath);

    if (audio.isDefined()) {
      auto& audioBuffer = audio.get().getBuffer();
      _sound.setBuffer(audioBuffer);

    } else {
      throw EngineException("Audio: " + audioPath + " not found");
    }
  }
};
