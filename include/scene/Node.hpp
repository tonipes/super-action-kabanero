#pragma once

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>

#include "physics/CollisionData.hpp"
#include "exception/EngineException.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "collection/Option.hpp"
#include "scene/Transform.hpp"
#include "scene/NodeAttachment.hpp"
#include "scene/BoundingBox.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"
#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "message/EventHandler.hpp"
#include "service/Services.hpp"
#include <typeinfo>
#include <iostream>
#include <string>
#include <memory>
#include "util/MatrixUtil.hpp"
#include "util/StringUtil.hpp"
#include "glm/gtx/string_cast.hpp"
#include "game/Behavior.hpp"
#include "scene/3D/Transform3D.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Node : public EventHandler, public std::enable_shared_from_this<Node> {
public:
  typedef Transform3D T;

  Node(std::string name);
  ~Node() {}

  auto isRenderOn() const -> bool;
  auto setRenderOn(bool b) -> void;

  auto name() const -> std::string;

  auto parent() const -> const Option<Node>;

  auto children() const -> const KBMap<std::string, std::shared_ptr<Node>>&;

  auto removeChild(std::string name) const -> const void;

  auto getNode(std::string path) const -> const Option<std::shared_ptr<Node>>;

  auto addChild(std::shared_ptr<Node> child) -> void;

  auto addAttachment(std::shared_ptr<NodeAttachment> attachment) -> void;

  template <typename AttachmentType>
  auto get() const -> Option<AttachmentType> {
     auto attachment =  _attachments.get<AttachmentType>();
    if (attachment.isDefined()) {
      return Some<AttachmentType>(std::dynamic_pointer_cast<AttachmentType>(attachment.get()));
    } else {
      return Option<AttachmentType>();
    }
  }

  template <typename AttachmentType>
  auto getShared() const -> Option<std::shared_ptr<AttachmentType>> {
    auto attachment = _attachments.get<AttachmentType>();
    if (attachment.isDefined()) {
      return Some<std::shared_ptr<AttachmentType>>(std::dynamic_pointer_cast< AttachmentType >( attachment.get() ));
    } else {
      return Option<std::shared_ptr<AttachmentType>>();
    }
  }

  auto transform() const -> glm::mat4x4;

  auto worldTransform() const -> glm::mat4x4;

  auto position() const -> const glm::vec3&;

  auto localPosition() const -> const glm::vec3;

  auto setLocalPosition(glm::vec3 v) -> void;

  auto localScale() const -> const glm::vec3;

  auto setLocalScale(glm::vec3 s) -> void;

  auto localRotation() const -> const glm::quat;

  auto setLocalRotation(glm::quat r) -> void;

  auto rotation() const -> glm::quat;

  auto scale() const -> glm::vec3;

  auto path() const -> std::string;

  auto setSleep(bool val) -> void;

  auto getAllowSleep() -> bool;

  auto setAllowSleep(bool isAllowed) -> void;

  auto isSleeping() const -> bool;

  auto wakeUp() -> void;

  template <typename BehaviorType, typename... Args>
  auto addBehavior(Args&&... args) -> std::shared_ptr<BehaviorType> {
    auto behavior = std::make_shared<BehaviorType>(this, std::forward<Args>(args)...);
    _behaviors += behavior;
    return behavior;
  }

  auto handleEvent(std::shared_ptr<Event> event) -> void;

  auto update(float delta) -> void;

  auto getRoot() -> std::shared_ptr<Node>;

  auto toBeDestroyed() -> bool;

  auto markToBeDestroyed() -> void;

  auto setBoundingBox(BoundingBox box) -> void;

  auto boundingBox() -> BoundingBox;

  auto localBoundingBox() -> BoundingBox;

protected:
  mutable bool _shouldUpdate = true;

  auto _update() const -> void;

  auto _addChildBB(BoundingBox box, glm::vec3 localPosition) -> void;

  auto _setUpdateFlag() const -> void ;

  auto _setParent(std::shared_ptr<Node> parent) -> void;

private:
  std::string _name;
  mutable KBMap<std::string, std::shared_ptr<Node>> _children;
  bool _render;
  bool _toBeDestroyed = false;
  bool _isSleeping = false;
  bool _allowSleep = true;
  Option<Node> _parent;
  KBTypeMap<std::shared_ptr<NodeAttachment>> _attachments;
  Transform3D _transform;
  BoundingBox _localBoundingBox;
  mutable BoundingBox _boundingBox;
  KBVector<std::shared_ptr<Behavior>> _behaviors;
  mutable glm::mat4x4 _worldTransform;
};
