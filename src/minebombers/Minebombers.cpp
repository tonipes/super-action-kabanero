#include "minebombers/Minebombers.hpp"

#include "service/Services.hpp"
#include "message/event/AudioClipEvent.hpp"
#include "message/event/AudioTrackEvent.hpp"

auto Minebombers::init() -> void {
  auto messagePublisher = Services::messagePublisher();
  messagePublisher->sendMessage(
    Message(
      "audioPlayer:clip/test_clip.ogg",
      std::make_shared<AudioClipEvent>(CLIP_PLAY)
    )
  );
  messagePublisher->sendMessage(
    Message(
      "audioPlayer:track/jazz",
      std::make_shared<AudioTrackEvent>(TRACK_CHANGE, "resources/audio/local_forecast.ogg")
    )
  );
  messagePublisher->sendMessage(
    Message(
      "audioPlayer:track/jazz",
      std::make_shared<AudioTrackEvent>(TRACK_PLAY)
    )
  );
}
