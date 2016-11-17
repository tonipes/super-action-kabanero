#pragma once

#include "message/Event.hpp"

enum AudioAction {
  PLAY,
  PAUSE,
  STOP
};

class AudioEvent: public Event {
public:
  AudioEvent(AudioAction action): _action(action) {}

  auto action() const -> AudioAction {
    return _action;
  }

private:
  AudioAction _action;
};
