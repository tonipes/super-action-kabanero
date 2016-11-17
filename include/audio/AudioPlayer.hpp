#pragma once

#include <SFML/Audio.hpp>
#include <regex>

#include "resource/resource/Audio.hpp"
#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "service/Services.hpp"
#include "audio/AudioClip.hpp"
#include "collection/mutable/KBMap.hpp"
#include "util/StringUtil.hpp"

/**
 * AudioPlayer class.
 */
class AudioPlayer: public MessageSubscriber {
public:
  AudioPlayer(std::string audioFolderPath) :
      MessageSubscriber("audioPlayer"),
      _audioFolderPath(audioFolderPath) {}

  ~AudioPlayer() {std::cout << "~AudioPlayer" << std::endl;}

  auto getEventHandler(const std::string& path) const -> EventHandler& override {
    if (!clips.contains(path)) {
      std::string fullPath = _audioFolderPath + path;
      clips[path] = std::make_shared<AudioClip>(fullPath);
    }
    return *clips[path];
  }

private:
  AudioPlayer(AudioPlayer& audioPlayer) : MessageSubscriber(audioPlayer.socket()){}
  bool isPlaying = false;
  sf::Sound _music;
  mutable KBMap<std::string, std::shared_ptr<AudioClip>> clips;
  // AudioClip testClip = AudioClip(_audioFolderPath + "local_forecast.ogg");
  std::string _audioFolderPath;
};
