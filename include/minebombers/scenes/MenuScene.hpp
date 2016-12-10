#pragma once

#include "scene/scene/GameScene.hpp"
#include "service/Services.hpp"
#include "minebombers/level/CaveGenerator.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/behaviors/CameraBehavior.hpp"
#include "minebombers/events/NewGameEvent.hpp"
#include "minebombers/util/NodeFactory.hpp"

class MenuScene {
public:
  static auto createScene(int seed) -> std::shared_ptr<GameScene> {

    const auto& messagePublisher = Services::messagePublisher();
    auto rootNode = std::make_shared<Node<Transform3D>>("world");

    auto scene = std::make_shared<GameScene>("gameScene", rootNode);

    auto w = 40;
    auto h = 28;

    auto caveGen = CaveGenerator(seed, w, h, 4, 3);
    auto tileMap = caveGen.generate();
    auto random = Services::random();

    auto levelCompiler = LevelCompiler(*random, scene->physWorld());
    levelCompiler.materializeLevel(tileMap, rootNode);

    auto cameraNode = std::make_shared<Node<Transform3D>>("camera");
    cameraNode->setLocalPosition(glm::vec3(w/2 + 0.5f, h/2 + 0.5, 0));

    cameraNode->addBehavior<CameraBehavior>(0.1f);
    auto visibilityAttachment = std::make_shared<VisibilityAttachment>(w, h, tileMap, true);
    cameraNode->addAttachment(visibilityAttachment);
    rootNode->addChild(cameraNode);

    SceneView sceneView(rootNode, cameraNode, Viewport(0, 0, 1.0, 1.0));
    scene->addSceneView(sceneView);

    auto menuNode = NodeFactory::createMainMenu();
    menuNode->setLocalPosition(glm::vec3(0, 0, 100.0f));
    rootNode->addChild(menuNode);


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