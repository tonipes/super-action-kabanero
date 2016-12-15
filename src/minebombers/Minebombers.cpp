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
#include "minebombers/scenes/MenuScene.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "collection/Option.hpp"
#include "physics/CollisionData.hpp"
#include "minebombers/util/NodeFactory.hpp"
// #include "minebombers/behaviors/EnemyOrbBehavior.hpp"

#include "physics/ContactListener.hpp"

auto Minebombers::init() -> void {

  auto messagePublisher = Services::messagePublisher();

  this->addEventReactor([&](NewGameEvent event) {
    auto seed = event.seed;
    auto numPlayers = event.numPlayers;
    if(numPlayers >= 1){
      Services::logger()->info("Create new MultiplayerScene");
      auto scene = MultiplayerScene::createScene(seed, numPlayers);
      addScene(scene);
      activateScene("gameScene");
    } else if (numPlayers <= 0){
      Services::logger()->info("Create new MenuScene");
      auto scene = MenuScene::createScene(seed);
      addScene(scene);
      activateScene("gameScene");
    } else if (numPlayers > 4){
      // Credits?
    }
  });


  int seed = 5;

  // auto gameScene = MultiplayerScene::createScene(seed, 1);
  auto menuScene = MenuScene::createScene(seed);
  addScene(menuScene);
  activateScene("gameScene");
}
