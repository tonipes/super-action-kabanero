#include "minebombers/Minebombers.hpp"

#include "service/Services.hpp"

#include "message/event/AudioClipEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "message/event/AudioTrackEvent.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/level/CaveGenerator.hpp"
#include "minebombers/behaviors/CameraBehavior.hpp"
#include "minebombers/behaviors/BulletHandlerBehaviour.hpp"
#include "minebombers/events/TestEvent.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "collection/Option.hpp"
#include "physics/CollisionData.hpp"
#include "minebombers/behaviors/EnemyOrbBehavior.hpp"

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

  _physWorld.SetContactListener(&_contactListener);

  auto rootNode = std::make_shared<Node<Transform3D>>("world");

  int seed = 5;

  auto random = StdLibRandom();
  random.seed(seed);

  auto caveGen = CaveGenerator(seed, 64, 64, 4, 3);
  auto tileMap = caveGen.generate();
  auto fogMap = std::make_shared<FogMap>();
  auto levelCompiler = LevelCompiler(random, _physWorld);

  levelCompiler.materializeLevel(tileMap, rootNode);
  levelCompiler.materializePlayer(tileMap, rootNode);
  levelCompiler.initFog(tileMap, fogMap, rootNode);


  Services::logger()->debug("num children: " + std::to_string(rootNode->children().values().length()));

  auto cameraNode = std::make_shared<Node<Transform3D>>("camera");
  cameraNode->setLocalPosition(glm::vec3(0, 0, 0));
  cameraNode->addBehavior<CameraBehavior>(0.2f);
  rootNode->addChild(cameraNode);

  // Just for testing
  auto enemyNode = std::make_shared<Node<Transform3D>>("test_enemy");
  enemyNode->setLocalPosition(glm::vec3(30, 24, 5));

  auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/orb_of_destruction");
  auto material_att = std::make_shared<CollisionMaterialAttachment>();
  
  material_att->collisionDamage = 1.0f;

  enemyNode->addBehavior<EnemyOrbBehavior>();
  enemyNode->addAttachment(sprite_att);
  enemyNode->addAttachment(material_att);

  auto bodyDef = std::make_shared<b2BodyDef>();

  bodyDef->type = b2_dynamicBody;
  bodyDef->position.Set(24, 30);
  bodyDef->allowSleep = false;
  bodyDef->fixedRotation = true;
  bodyDef->linearDamping = 0.5f;

  auto shape = std::make_shared<b2CircleShape>();
  shape->m_p.Set(0, 0);
  shape->m_radius = 0.2f;
  Services::messagePublisher()->sendMessage(Message("game", std::make_shared<CreateNodeEvent>(
    "world/bulletHandler", bodyDef, shape, enemyNode
  )));

  auto bulletHandler = std::make_shared<Node<Transform3D>>("bulletHandler");
  bulletHandler->setLocalPosition(glm::vec3(0, 0, 0));
  bulletHandler->addBehavior<BulletHandlerBehaviour>(_physWorld);
  rootNode->addChild(bulletHandler);

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

  // Can't add directly under the rood node
  addEventReactor([&](CreateNodeEvent event) {
    auto rootNode = activeScenes[0]->rootNode(); // ???
    auto path = event.parentPath();

    Services::logger()->debug(path);

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
        body->CreateFixture(event.shape().get(), 1.0f);


        // event.node()->setPhysics(body);

        auto physAttachment = std::make_shared<PhysicsAttachment>(body);
        event.node()->addAttachment(physAttachment);

        parent->addChild(event.node());
        Services::logger()->debug("path: " + event.node()->path());

        auto material_att = event.node()->getShared<CollisionMaterialAttachment>();
        if(material_att.isDefined()){

          Services::logger()->debug("Add collisionData");
          auto collisionData = new CollisionData(event.node()->path(), material_att.get());
          body->SetUserData(collisionData);
        }

      }
    }
    Services::logger()->debug("CreateNode");

  });
}
