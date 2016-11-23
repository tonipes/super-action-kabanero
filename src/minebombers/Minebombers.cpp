#include "minebombers/Minebombers.hpp"

#include "service/Services.hpp"
#include "message/event/AudioEvent.hpp"

auto Minebombers::init() -> void {
  Services::messagePublisher()->sendMessage(
    Message(
      "audioPlayer:local_forecast.ogg",
      std::make_shared<AudioEvent>(PLAY)
    )
  );
}
