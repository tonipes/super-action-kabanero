#pragma once

#include "message/Event.hpp"

enum AudioAction {
  PLAY_MUSIC,
  STOP_MUSIC,
  CHANGE_MUSIC,
  PLAY_SOUND
};

class AudioEvent: public Event {
public:
  AudioEvent(AudioAction action, std::string name): _name(name), _action(action) {}

  auto getName() -> std::string& {
    return _name;
  }

  auto getAction() -> AudioAction {
    return _action;
  }

private:
  AudioAction _action;
  std::string _name;

};
