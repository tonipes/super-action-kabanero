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
  SceneGraph(std::shared_ptr<Node> rootNode): _root(rootNode) {}
  ~SceneGraph() {}

  auto init() -> void override {}

  auto update(double delta) -> void override {}

  auto getSceneViews() -> KBVector<SceneView> {
    return _sceneViews;
  }

  auto root() -> std::shared_ptr<Node> {
    return _root;
  }

  auto addSceneView(SceneView sceneView) -> void {
    _sceneViews += sceneView;
  }

private:
  std::shared_ptr<Node> _root;
  KBVector<SceneView> _sceneViews;
};
