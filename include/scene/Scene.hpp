#pragma once

#include "game/Updateable.hpp"
#include "scene/SceneGraph.hpp"
#include "scene/SceneView.hpp"
#include "message/MessageSubscriber.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * Scene interface.
 */
template <typename T>
class Scene : public Updateable, public MessageSubscriber {
public:
  Scene(
      std::string name,
      std::shared_ptr<Node<T>> rootNode) :
    MessageSubscriber(name),
    _graph(rootNode) {}
  virtual ~Scene() {}

  auto getSceneViews() -> KBVector<SceneView<T>> {
    return _graph.getSceneViews();
  }

  auto rootNode() -> Node<T>& {
    return _graph.root();
  }

  auto addSceneView(SceneView<T> sceneView) -> void {
    _graph.addSceneView(sceneView);
  }

protected:
  SceneGraph<T> _graph;
};
