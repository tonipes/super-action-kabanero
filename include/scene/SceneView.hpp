#pragma once

#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "graphics/Viewport.hpp"

#include <memory>

class SceneView {
public:
  SceneView(
      std::shared_ptr<Node> rootNode,
      std::shared_ptr<Node> cameraNode,
      Viewport viewport) :
    _rootNode(rootNode),
    _cameraNode(cameraNode),
    _viewport(viewport) { }

  auto cameraNode() const -> std::shared_ptr<Node> {
    return _cameraNode;
  }

  auto rootNode() const -> std::shared_ptr<Node> {
    return _rootNode;
  }

  auto viewport() const -> Viewport {
    return _viewport;
  }

private:
  std::shared_ptr<Node> _rootNode;
  std::shared_ptr<Node> _cameraNode;
  Viewport _viewport;
};
