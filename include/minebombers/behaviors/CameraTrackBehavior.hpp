#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "minebombers/attachments/VisibilityAttachment.hpp"

class CameraTrackBehavior : public Behavior {
public:
  CameraTrackBehavior(
      Node* node,
      const std::string& trackedNodePath,
      float camSpeed = 0.2f) : _trackedNodePath(trackedNodePath), _camSpeed(camSpeed) {}

  auto update(float delta, std::shared_ptr<Node> node) -> void override {
    const auto pos = node->position();

    if (_trackedNode.isEmpty()) {
      auto root = node->getRoot();
      _trackedNode = root->getNode(_trackedNodePath);
    }
    _trackedNode.foreach([&](auto nodePtr) {
      const auto targetPos = nodePtr->position();
      auto moveDirection = targetPos - pos;
      node->setLocalPosition(pos + moveDirection * _camSpeed);

      const auto& visibilityAttachment = node->get<VisibilityAttachment>();
      if (visibilityAttachment.isDefined()) {
        visibilityAttachment.get().markVisited(round(targetPos.x), round(targetPos.y));
      }
    });
  }
private:
  std::string _trackedNodePath;
  Option<std::shared_ptr<Node>> _trackedNode;
  float _camSpeed;
};
