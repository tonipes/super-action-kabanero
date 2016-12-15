#include "minebombers/Minebombers.hpp"

#include "service/Services.hpp"

#include "message/event/AudioClipEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "message/event/AudioTrackEvent.hpp"
#include "minebombers/events/NewGameEvent.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/level/CaveGenerator.hpp"
#include "minebombers/attachments/VisibilityAttachment.hpp"
#include "minebombers/events/TestEvent.hpp"
#include "minebombers/events/TestEvent.hpp"
#include "minebombers/scenes/MultiplayerScene.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "collection/Option.hpp"
#include "physics/CollisionData.hpp"
// #include "minebombers/behaviors/EnemyOrbBehavior.hpp"

#include "physics/ContactListener.hpp"

auto Minebombers::init() -> void {

  auto messagePublisher = Services::messagePublisher();

  this->addEventReactor([&](NewGameEvent event) {
    auto seed = event.seed;
    auto numPlayers = event.numPlayers;

    auto scene = MultiplayerScene::createScene(seed, numPlayers);
    addScene(scene);
    activateScene("gameScene");
  });


  messagePublisher->sendMessage(
    Message(
      "audioPlayer:track/jazz",
      std::make_shared<AudioTrackEvent>(TRACK_CHANGE, "resources/audio/local_forecast.ogg")
    )
  );
  // messagePublisher->sendMessage(
  //   Message(
  //     "audioPlayer:track/jazz",
  //     std::make_shared<AudioTrackEvent>(TRACK_PLAY)
  //   )
  // );

  int seed = 5;

  auto gameScene = MultiplayerScene::createScene(seed, 1);
  addScene(gameScene);
  activateScene("gameScene");
}
