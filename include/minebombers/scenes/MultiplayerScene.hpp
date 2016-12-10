#pragma once

#include "scene/scene/GameScene.hpp"
#include "service/Services.hpp"
#include "minebombers/level/CaveGenerator.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/behaviors/CameraBehavior.hpp"
#include "minebombers/events/NewGameEvent.hpp"

class MultiplayerScene {
public:
  static auto createScene(int seed, int numPlayers = 1) -> std::shared_ptr<GameScene> {
    const auto& messagePublisher = Services::messagePublisher();
    auto rootNode = std::make_shared<Node<Transform3D>>("world");

    auto scene = std::make_shared<GameScene>("gameScene", rootNode);

    auto w = 64;
    auto h = 64;

    auto caveGen = CaveGenerator(seed, w, h, 4, 3);
    auto tileMap = caveGen.generate();
    auto random = Services::random();

    auto levelCompiler = LevelCompiler(*random, scene->physWorld());
    levelCompiler.materializeLevel(tileMap, rootNode);

    auto bulletBag = std::make_shared<Node<Transform3D>>("bullets");
    bulletBag->setLocalPosition(glm::vec3(0, 0, 0));
    rootNode->addChild(bulletBag);

    std::vector<std::shared_ptr<Node<Transform3D>>> cameras;

    for (auto i = 1; i <= numPlayers; i++) {
      auto id = std::to_string(i);
      levelCompiler.materializePlayer(tileMap, rootNode, "player" + id, "world/camera" + id);

      auto cameraNode = std::make_shared<Node<Transform3D>>("camera" + id);
      cameraNode->addBehavior<CameraBehavior>(0.2f);
      auto visibilityAttachment = std::make_shared<VisibilityAttachment>(w, h, tileMap);
      cameraNode->addAttachment(visibilityAttachment);
      rootNode->addChild(cameraNode);

      cameras.push_back(cameraNode);
    }

    messagePublisher->sendMessage(
      Message(
        "audioPlayer:track/track",
        std::make_shared<AudioTrackEvent>(TRACK_CHANGE, "resources/audio/music_highway.ogg")
      )
    );
    messagePublisher->sendMessage(
      Message(
        "audioPlayer:track/track",
        std::make_shared<AudioTrackEvent>(TRACK_PLAY)
      )
    );

    if (numPlayers == 1) {
      SceneView sceneView(rootNode, cameras[0], Viewport(0, 0, 1.0, 1.0));
      scene->addSceneView(sceneView);
    } else if (numPlayers == 2) {
      SceneView sceneView(rootNode, cameras[0], Viewport(0, 0, 0.5, 1.0));
      scene->addSceneView(sceneView);

      SceneView sceneView2(rootNode, cameras[1], Viewport(0.5, 0, 0.5, 1.0));
      scene->addSceneView(sceneView2);
    } else if (numPlayers == 3) {
      SceneView sceneView(rootNode, cameras[0], Viewport(0, 0, 0.5, 0.5));
      scene->addSceneView(sceneView);

      SceneView sceneView2(rootNode, cameras[1], Viewport(0.5, 0, 0.5, 0.5));
      scene->addSceneView(sceneView2);

      SceneView sceneView3(rootNode, cameras[2], Viewport(0.0, 0.5, 0.5, 0.5));
      scene->addSceneView(sceneView3);
    } else {
      SceneView sceneView(rootNode, cameras[0], Viewport(0, 0, 0.5, 0.5));
      scene->addSceneView(sceneView);

      SceneView sceneView2(rootNode, cameras[1], Viewport(0.5, 0, 0.5, 0.5));
      scene->addSceneView(sceneView2);

      SceneView sceneView3(rootNode, cameras[2], Viewport(0.0, 0.5, 0.5, 0.5));
      scene->addSceneView(sceneView3);

      SceneView sceneView4(rootNode, cameras[3], Viewport(0.5, 0.5, 0.5, 0.5));
      scene->addSceneView(sceneView4);
    }

    scene->addEventReactor([messagePublisher](GameInputEvent event) {
      auto action = event.action();
      if (action == NUM_1) {
        std::cout << "Send new game event 1" << std::endl;
        messagePublisher->sendMessage(Message(
          "game",
          std::make_shared<NewGameEvent>(5, 1)
          )
        );
      } else if (action == NUM_2) {
        std::cout << "Send new game event 2" << std::endl;
        messagePublisher->sendMessage(Message(
          "game",
          std::make_shared<NewGameEvent>(15, 2)
          )
        );
      } else if (action == NUM_3) {
        std::cout << "Send new game event 3" << std::endl;
        messagePublisher->sendMessage(Message(
          "game",
          std::make_shared<NewGameEvent>(21, 3)
          )
        );
      } else if (action == NUM_4) {
        std::cout << "Send new game event 4" << std::endl;
        messagePublisher->sendMessage(Message(
          "game",
          std::make_shared<NewGameEvent>(40, 4)
          )
        );
      }
    });

    return scene;
  }
private:
};
