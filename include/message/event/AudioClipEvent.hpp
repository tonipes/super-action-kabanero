#pragma once

#include "message/Event.hpp"

enum AudioClipAction {
  CLIP_PLAY
};

class AudioClipEvent: public Event {
public:
  AudioClipEvent(AudioClipAction action): _action(action) {}

  auto action() const -> AudioClipAction {
    return _action;
  }

private:
  AudioClipAction _action;
};
