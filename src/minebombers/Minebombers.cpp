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

  auto rootNode = std::make_shared<Node<Transform3D>>("world");

  auto terrainNode = std::make_shared<Node<Transform3D>>("terrain");
  terrainNode->addAttachment(std::make_shared<SpriteAttachment>("test-ground/lava0"));

  auto terrainNode1 = std::make_shared<Node<Transform3D>>("terrain");
  terrainNode1->addAttachment(std::make_shared<SpriteAttachment>("test-ground/lava1"));
  terrainNode1->setLocalPosition(glm::vec3(32, 0, 0));

  auto terrainNode2 = std::make_shared<Node<Transform3D>>("terrain");
  terrainNode2->addAttachment(std::make_shared<SpriteAttachment>("test-ground/lava2"));
  terrainNode2->setLocalPosition(glm::vec3(0, 64, 0));

  auto terrainNode3 = std::make_shared<Node<Transform3D>>("terrain");
  terrainNode3->addAttachment(std::make_shared<SpriteAttachment>("test-ground/lava3"));
  terrainNode3->setLocalPosition(glm::vec3(-32, -32, 0));

  rootNode->addChild(terrainNode);
  rootNode->addChild(terrainNode1);
  rootNode->addChild(terrainNode2);
  rootNode->addChild(terrainNode3);

  auto cameraNode = std::make_shared<Node<Transform3D>>("camera");
  rootNode->addChild(cameraNode);

  auto scene = std::make_shared<GameScene<Transform3D>>("gameScene", rootNode);

  SceneView<Transform3D> sceneView(rootNode, cameraNode, Viewport(0, 0, 1, 1));
  scene->addSceneView(sceneView);

  addScene(scene);
}
