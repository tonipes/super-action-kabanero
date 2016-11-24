#pragma once

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "collection/Option.hpp"
#include "scene/Transform.hpp"
#include "scene/NodeAttachment.hpp"
#include <typeinfo>
#include <iostream>
#include <memory>
#include "util/MatrixUtil.hpp"
#include "glm/gtx/string_cast.hpp"

/**
 * Node interface.
 */
template <typename T>
class Node : public std::enable_shared_from_this<Node<T>> {
public:
  Node(std::string name) : _name(name) {}

  auto name() const -> std::string {
    return _name;
  }

  auto parent() const -> const Option<Node> {
    return _parent;
  }

  auto children() const -> const KBVector<Node>& {
    return _children;
  }

  auto addChild(std::shared_ptr<Node> child) -> void {
    _children += child;
    child->_setParent(this->shared_from_this());
    child->_setUpdateFlag();
  }

  auto addAttachment(std::shared_ptr<NodeAttachment> attachment) -> void {
    _attachments[typeid(*attachment)] = attachment;
  }

  template <typename AttachmentType>
  auto get() -> Option<AttachmentType> {
    _attachments.get<AttachmentType>();
  }

  auto transform() const -> typename T::matrixType {
    return _transform.matrix();
  }

  auto worldTransform() const -> typename T::matrixType {
    if (_shouldUpdate) {
      _update();
    }
    return _worldTransform;
  }

  auto position() const -> typename T::vectorType {
    return MatrixUtil::getTransform(worldTransform());
  }

  auto localPosition() const -> const typename T::vectorType {
    return _transform.position();
  }

  auto setLocalPosition(typename T::vectorType v) -> void {
    _transform.setPosition(v);
    _setUpdateFlag();
  }

  auto localScale() const -> const typename T::vectorType {
    return _transform.scale();
  }

  auto setLocalScale(typename T::vectorType v) -> void {
    _transform.setScale(v);
    _setUpdateFlag();
  }

  auto localRotation() const -> const typename T::rotationType {
    return _transform.rotation();
  }

  auto setLocalRotation(typename T::rotationType r) -> void {
    _transform.setRotation(r);
    _setUpdateFlag();
  }

  auto rotation() const -> typename T::rotationType {
    return _parent.map([&](const auto& parentNode) {
      return parentNode.rotation() * localRotation();
    }).getOrElse(localRotation());
  }

  auto scale() const -> typename T::vectorType {
    return _parent.map([&](const auto& parentNode) {
      return parentNode.scale() * localScale();
    }).getOrElse(localScale());
  }

  auto operator==(Node<T> other) -> bool {
    return true;
  }

protected:
  mutable bool _shouldUpdate = true;

  auto _update() const -> void {
    _worldTransform = _parent.map([&](const auto& parentNode) {
      return parentNode.worldTransform() * _transform.matrix();
    }).getOrElse(_transform.matrix());
    _setUpdateFlag();
  }

  auto _setUpdateFlag() const -> void {
    if (!_shouldUpdate) {
      _shouldUpdate = true;
      _children.foreach([](auto childPtr) {
        if (auto child = childPtr.lock()) {
          child->_setUpdateFlag();
        }
      });
    }
  }

  auto _setParent(std::shared_ptr<Node> parent) -> void {
    _parent = Some(parent);
    this->_setUpdateFlag();
  }

private:
  std::string _name;
  KBVector<std::weak_ptr<Node>> _children;
  Option<Node> _parent;
  KBTypeMap<std::shared_ptr<NodeAttachment>> _attachments;
  T _transform;
  mutable typename T::matrixType _worldTransform;
};

template <typename T>
auto operator<<(std::ostream& os, const Node<T>& node) -> std::ostream& {
  os << "Node: " <<
    "  name: " << node.name() << std::endl <<
    "  position: " << glm::to_string(node.position()) << std::endl <<
    "  rotation: " << glm::to_string(node.rotation()) << std::endl <<
    "  scale: " << glm::to_string(node.scale());

  return os;

}
