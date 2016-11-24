#pragma once

#include "scene/Node.hpp"
#include "graphics/Viewport.hpp"

#include <memory>

template <typename T>
class SceneView {
public:
  SceneView(
      std::shared_ptr<Node<T>> rootNode,
      std::shared_ptr<Node<T>> cameraNode,
      Viewport viewport) :
    _rootNode(rootNode),
    _cameraNode(cameraNode),
    _viewport(viewport) { }

  auto cameraNode() -> std::shared_ptr<Node<T>> {
    return _cameraNode;
  }

  auto rootNode() -> std::shared_ptr<Node<T>> {
    return _rootNode;
  }

  auto viewport() -> Viewport {
    return _viewport;
  }

private:
  std::shared_ptr<Node<T>> _rootNode;
  std::shared_ptr<Node<T>> _cameraNode;
  Viewport _viewport;
};
