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
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "collection/Option.hpp"
#include "physics/CollisionData.hpp"
#include "minebombers/ui/MainMenu.hpp"
// #include "minebombers/behaviors/EnemyOrbBehavior.hpp"

#include "physics/ContactListener.hpp"

auto Minebombers::init() -> void {

  auto messagePublisher = Services::messagePublisher();
  messagePublisher->sendMessage(
    Message(
      "audioPlayer:track/jazz",
      std::make_shared<AudioTrackEvent>(TRACK_CHANGE, "resources/audio/local_forecast.ogg")
    )
  );

  _physWorld.SetContactListener(&_contactListener);

  auto rootNode = std::make_shared<Node<Transform3D>>("world");

  Services::logger()->debug("num children: " + std::to_string(rootNode->children().values().length()));

  auto cameraNode = std::make_shared<Node<Transform3D>>("camera");
  cameraNode->setLocalPosition(glm::vec3(0, 0, 0));
  cameraNode->addBehavior<CameraBehavior>(0.2f);
  rootNode->addChild(cameraNode);

  auto bulletBag = std::make_shared<Node<Transform3D>>("bullets");
  bulletBag->setLocalPosition(glm::vec3(0, 0, 0));
  rootNode->addChild(bulletBag);

  auto scene = std::make_shared<GameScene<Transform3D>>("gameScene", rootNode);

  SceneView<Transform3D> sceneView(rootNode, cameraNode, Viewport(0, 0, 1, 1));
  scene->addSceneView(sceneView);

  addScene(scene);

  messagePublisher->sendMessage(
    Message(
      "gameScene:world/camera",
      std::make_shared<TestEvent>(A)
    )
  );
  messagePublisher->sendMessage(
    Message(
      "gameScene:world/camera",
      std::make_shared<TestEvent>(B)
    )
  );

  auto menu = MainMenu();
  auto ui = menu.init();
  rootNode->addChild(ui);

  // These reactors should probably be somewhere further down in the tree.
  // Uses only the first scene in activeScenes
  addEventReactor([&](DestroyNodeEvent event) {
    auto rootNode = activeScenes[0]->rootNode(); // ???

    auto path = event.path();

    // Bit hacky. Can't destroy root
    // Needs to drop the first part from the path
    auto i = path.find('/');
    if (i != std::string::npos) {
      auto p = path.substr(i+1, path.length());
      auto node = rootNode->getNode(p);
      if(node.isDefined()){
        if(!node.get()->toBeDestroyed()){
          node.get()->markToBeDestroyed();
          _toBeDestryed += node.get();
        }
      }
    }

  });

  addEventReactor([&](GameTypeSelectedEvent event) {
    Minebombers::startGame(event.getType());
  });

  // Can't add directly under the rood node
  addEventReactor([&](CreateNodeEvent event) {
    auto rootNode = activeScenes[0]->rootNode(); // ???
    auto path = event.parentPath();

    // Bit hacky. Can't directly under the rood node root
    // Needs to drop the first part from the path
    auto i = path.find('/');
    if (i != std::string::npos) {
      auto p = path.substr(i+1, path.length());
      auto parentOption = rootNode->getNode(p);
      if(parentOption.isDefined()){
        auto parent = parentOption.get();
        // Shamelesly modifying tree and world directly.
        b2Body* body = _physWorld.CreateBody(event.bodyDef().get());
        body->CreateFixture(event.fixtureDef().get());

        delete event.fixtureDef().get()->shape;

        auto physAttachment = std::make_shared<PhysicsAttachment>(body);
        event.node()->addAttachment(physAttachment);

        parent->addChild(event.node());

        auto material_att = event.node()->getShared<CollisionMaterialAttachment>();
        if(material_att.isDefined()){
          auto collisionData = new CollisionData(event.node()->path(), material_att.get());
          body->SetUserData(collisionData);
        }

      }
    }
  });
  Services::messagePublisher()->sendMessage(Message(
    "all",
    std::make_shared<ButtonSelectedEvent>(0, false)
  ));
}
auto Minebombers::startGame(GameType gameType) -> void {
    auto rootNode = activeScenes[0]->rootNode();
    rootNode->removeChild("menu");
    int seed = static_cast<unsigned int>( time(NULL) );

    auto random = StdLibRandom();
    random.seed(seed);

    auto caveGen = CaveGenerator(seed, 32, 32, 4, 3);
    auto tileMap = caveGen.generate();
    auto fogMap = std::make_shared<FogMap>();
    auto levelCompiler = LevelCompiler(random, _physWorld);

    levelCompiler.materializeLevel(tileMap, rootNode);
    levelCompiler.materializePlayer(tileMap, rootNode);
    levelCompiler.initFog(tileMap, fogMap, rootNode);
}
