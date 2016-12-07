#pragma once

#include <SFML/Audio.hpp>
#include <regex>

#include "resource/resource/Audio.hpp"
#include "exception/EngineException.hpp"
#include "message/Message.hpp"
#include "message/MessageSubscriber.hpp"
#include "service/Services.hpp"
#include "audio/AudioClip.hpp"
#include "audio/AudioTrack.hpp"
#include "collection/mutable/KBMap.hpp"
#include "util/StringUtil.hpp"

/**
 * AudioPlayer class.
 */
class AudioPlayer: public MessageSubscriber, public EventHandler {
public:
  AudioPlayer(std::string audioFolderPath, int maxClipCount = 16) :
      MessageSubscriber("audioPlayer"),
      _maxClipCount(maxClipCount),
      _audioFolderPath(audioFolderPath) { }

  ~AudioPlayer() { std::cout << "~AudioPlayer" << std::endl; }

  auto getEventHandler(const std::string& path) -> EventHandler& override {
    auto parts = split(path, '/');
    auto type = parts[0];
    auto id = parts[1];
    // std::cout << "type: " << type << '\n';
    // std::cout << "id: " << id << '\n';

    if (type.compare("track") == 0) { // Track event
      // std::cout << "track" << '\n';

      if (!tracks.contains(id)) {
        tracks[id] = std::make_shared<AudioTrack>();
      }
      return *tracks[id];

    } else if (type.compare("clip") == 0) { // Clip event
      auto len = (int) clips.length();

      if(len == _maxClipCount - 1){
        clips.remove(0);
      }

      std::string fullPath = _audioFolderPath + id;
      clips += std::make_unique<AudioClip>(fullPath);

      return *clips[clips.length()-1];

    } else {
      throw EngineException("Error with audio event");
    }
  }

  auto getAllEventHandlers() -> KBVector<EventHandler> override {
    auto v = KBVector<EventHandler>();
    v += *this;
    return v;
  }

private:
  AudioPlayer(AudioPlayer& audioPlayer) : MessageSubscriber(audioPlayer.socket()){}

  // mutable std::vector<int> clips;
  // mutable std::vector<std::unique_ptr<AudioClip>> clips;
  mutable KBVector<std::unique_ptr<AudioClip>> clips;

  mutable KBMap<std::string, std::shared_ptr<AudioTrack>> tracks;

  std::string _audioFolderPath;
  int _maxClipCount;
};
