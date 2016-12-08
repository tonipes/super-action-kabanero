#pragma once

#include "scene/scene/GameScene.hpp"
#include "service/Services.hpp"
#include "minebombers/level/CaveGenerator.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/level/LevelCompiler.hpp"
#include "minebombers/behaviors/CameraBehavior.hpp"

class MultiplayerScene {
public:
  static auto createScene(int seed) -> std::shared_ptr<GameScene> {
    const auto& messagePublisher = Services::messagePublisher();
    auto rootNode = std::make_shared<Node<Transform3D>>("world");

    auto scene = std::make_shared<GameScene>("gameScene", rootNode);

    auto caveGen = CaveGenerator(seed, 128, 128, 4, 3);
    auto tileMap = caveGen.generate();

    auto random = Services::random();

    auto levelCompiler = LevelCompiler(*random, scene->physWorld());
    levelCompiler.materializeLevel(tileMap, rootNode);
    levelCompiler.materializePlayer(tileMap, rootNode);

    auto cameraNode = std::make_shared<Node<Transform3D>>("camera");
    cameraNode->setLocalPosition(glm::vec3(0, 0, 0));
    cameraNode->addBehavior<CameraBehavior>(0.2f);
    rootNode->addChild(cameraNode);

    auto bulletBag = std::make_shared<Node<Transform3D>>("bullets");
    bulletBag->setLocalPosition(glm::vec3(0, 0, 0));
    rootNode->addChild(bulletBag);

    SceneView sceneView(rootNode, cameraNode, Viewport(0, 0, 0.5, 0.5));
    scene->addSceneView(sceneView);

    SceneView sceneView2(rootNode, cameraNode, Viewport(0.5, 0, 0.5, 0.5));
    scene->addSceneView(sceneView2);

    SceneView sceneView3(rootNode, cameraNode, Viewport(0.0, 0.5, 0.5, 0.5));
    scene->addSceneView(sceneView3);

    SceneView sceneView4(rootNode, cameraNode, Viewport(0.5, 0.5, 0.5, 0.5));
    scene->addSceneView(sceneView4);

    return scene;
  }
private:
};
