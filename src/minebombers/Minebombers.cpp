#include "minebombers/Minebombers.hpp"

#include "service/Services.hpp"

#include "message/event/AudioClipEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "message/event/AudioTrackEvent.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/level/CaveGenerator.hpp"
#include "minebombers/behaviors/CameraBehavior.hpp"
#include "minebombers/events/TestEvent.hpp"
#include "minebombers/scenes/MultiplayerScene.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "collection/Option.hpp"
#include "physics/CollisionData.hpp"
// #include "minebombers/behaviors/EnemyOrbBehavior.hpp"

#include "physics/ContactListener.hpp"

auto Minebombers::init() -> void {

  auto messagePublisher = Services::messagePublisher();
  // messagePublisher->sendMessage(
  //   Message(
  //     "audioPlayer:clip/test_clip.ogg",
  //     std::make_shared<AudioClipEvent>(CLIP_PLAY)
  //   )
  // );
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

  // auto rootNode = std::make_shared<Node<Transform3D>>("world");
  //
  int seed = 5;

  auto gameScene = MultiplayerScene::createScene(seed);
  addScene(gameScene);
}
