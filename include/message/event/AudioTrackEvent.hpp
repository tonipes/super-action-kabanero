#pragma once

#include "message/Event.hpp"

enum AudioTrackAction {
  TRACK_PLAY,
  TRACK_CHANGE,
  TRACK_PAUSE,
  TRACK_STOP
};

class AudioTrackEvent: public Event {
public:
  AudioTrackEvent(AudioTrackAction action, const std::string& audioPath = ""):
    _action(action),
    _audioPath(audioPath) {}

  auto action() const -> AudioTrackAction {
    return _action;
  }

  auto audioPath() const -> std::string {
    return _audioPath;
  }

private:
  AudioTrackAction _action;
  std::string _audioPath;
};
