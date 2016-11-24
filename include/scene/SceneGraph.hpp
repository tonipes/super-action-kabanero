#pragma once

#include "game/Updateable.hpp"
#include "scene/Node.hpp"
#include "scene/SceneView.hpp"
#include "collection/mutable/KBVector.hpp"

/**
 * SceneGraph class.
 */
template <typename T>
class SceneGraph : public Updateable {
public:
  SceneGraph(std::shared_ptr<Node<T>> rootNode): _root(rootNode) {}
  ~SceneGraph() {}

  auto init() -> void override {}

  auto update(double delta) -> void override {}

  auto getSceneViews() -> KBVector<SceneView<T>> {
    return _sceneViews;
  }

  auto root() -> Node<T>& {
    return _root;
  }

  auto addSceneView(SceneView<T> sceneView) -> void {
    _sceneViews += sceneView;
  }

private:
  std::shared_ptr<Node<T>> _root;
  KBVector<SceneView<T>> _sceneViews;
};
