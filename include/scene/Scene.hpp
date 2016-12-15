#pragma once

#include "game/Updateable.hpp"
#include "scene/SceneGraph.hpp"
#include "scene/SceneView.hpp"
#include "message/MessageSubscriber.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * Scene interface.
 */
class Scene : public Updateable, public MessageSubscriber {
public:
  Scene(
      std::string name,
      std::shared_ptr<Node> rootNode) :
    MessageSubscriber(name),
    _graph(rootNode) {}
  virtual ~Scene() {}

  auto getSceneViews() -> KBVector<SceneView> {
    return _graph.getSceneViews();
  }

  auto rootNode() -> std::shared_ptr<Node> {
    return _graph.root();
  }

  auto addSceneView(SceneView sceneView) -> void {
    _graph.addSceneView(sceneView);
  }

protected:
  SceneGraph _graph;
};
