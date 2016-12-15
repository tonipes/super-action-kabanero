#include "scene/Node.hpp"

Node::Node(std::string name) : _name(name), _render(true) {}

auto Node::isRenderOn() const -> bool { return _render; }
auto Node::setRenderOn(bool b) -> void { _render = b; }

auto Node::name() const -> std::string {
  return _name;
}

auto Node::parent() const -> const Option<Node> {
  return _parent;
}

auto Node::children() const -> const KBMap<std::string, std::shared_ptr<Node>>& {
  return _children;
}

auto Node::removeChild(std::string name) const -> const void {
  _children.remove(name);
}

auto Node::getNode(std::string path) const -> const Option<std::shared_ptr<Node>> {
  auto i = path.find('/');
  if (i != std::string::npos){
    auto first_token = path.substr(0, i);
    auto rest = path.substr(i+1, path.length());
    auto c = children().get(first_token);
    if(c.isDefined()){
      return c.get()->getNode(rest);
    } else {
      return Option<std::shared_ptr<Node>>();
    }
  } else{
    return _children.get(path);
  }
}

auto Node::addChild(std::shared_ptr<Node> child) -> void {
  this->wakeUp();
  _children.insert(child->name(), child);
  child->_setParent(this->shared_from_this());

  auto materialAttachment = child->template getShared<CollisionMaterialAttachment>();
  auto physicsAttachment = child->template get<PhysicsAttachment>();

  if(physicsAttachment.isDefined() && materialAttachment.isDefined()){
    auto collisionData = new CollisionData(child->path(), materialAttachment.get());
    physicsAttachment.get().body()->SetUserData(collisionData);
  }

  _addChildBB(child->boundingBox(), child->localPosition());

  child->_setUpdateFlag();
  if (!child->getAllowSleep()) {
    this->setAllowSleep(false);
  }
}

auto Node::addAttachment(std::shared_ptr<NodeAttachment> attachment) -> void {
  _attachments[typeid(*attachment)] = attachment;
}

auto Node::transform() const -> glm::mat4x4 {
  return _transform.matrix();
}

auto Node::worldTransform() const -> typename Transform3D::matrixType {
  if (_shouldUpdate) {
    _update();
  }
  return _worldTransform;
}

auto Node::position() const -> const glm::vec3& {
  glm::vec3 v = glm::vec3(worldTransform()[3]);
  return v;
}

auto Node::localPosition() const -> const glm::vec3 {
  return _transform.position();
}

auto Node::setLocalPosition(glm::vec3 v) -> void {
  _transform.setPosition(v);
  _setUpdateFlag();
  if (_parent.isDefined()) {
    _parent.get()._addChildBB(boundingBox(), localPosition());
  }

}

auto Node::localScale() const -> const typename T::vectorType {
  return _transform.scale();
}

auto Node::setLocalScale(typename T::vectorType v) -> void {
  _transform.setScale(v);
  _setUpdateFlag();
}

auto Node::localRotation() const -> const typename T::rotationType {
  return _transform.rotation();
}

auto Node::setLocalRotation(typename T::rotationType r) -> void {
  _transform.setRotation(r);
  _setUpdateFlag();
}

auto Node::rotation() const -> typename T::rotationType {
  return _parent.map([&](const auto& parentNode) {
    return parentNode.rotation() * localRotation();
  }).getOrElse(localRotation());
}

auto Node::scale() const -> typename T::vectorType {
  return _parent.map([&](const auto& parentNode) {
    return parentNode.scale() * localScale();
  }).getOrElse(localScale());
}

auto Node::path() const -> std::string {
  return _parent.map([&](const auto& parentNode) {
    return parentNode.path() + "/" + name();
  }).getOrElse(name());
}

auto Node::setSleep(bool val) -> void {
  if (_allowSleep) {
    _isSleeping = true;
  }
}

auto Node::getAllowSleep() -> bool {
  return _allowSleep;
}

auto Node::setAllowSleep(bool isAllowed) -> void {
  _allowSleep = isAllowed;
  if (!isAllowed && _parent.isDefined()) {
    _parent.get().setAllowSleep(isAllowed);
  }
}

auto Node::isSleeping() const -> bool {
  return _isSleeping;
}

auto Node::wakeUp() -> void {
  _isSleeping = false;
  if (_parent.isDefined()) {
    _parent.get().wakeUp();
  }
 }


auto Node::handleEvent(std::shared_ptr<Event> event) -> void {
  this->wakeUp();
  if (reactors.contains(typeid(*event))) {
    reactors[typeid(*event)](event);
  }
}

auto Node::update(float delta) -> void {
  if (!_isSleeping) {
    auto allSleeping = true;
    _children.values().foreach([&](auto child) {
      child->update(delta);
      if (!child->isSleeping()) {
        allSleeping = false;
      }
    });

    const auto& physAttachment = this->get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {
      auto pos = phys.position();
      if (_parent.isDefined()) {
        const auto& parentPos = _parent.get().position();
        pos.x -= parentPos.x;
        pos.y -= parentPos.y;
      }
      this->setLocalPosition(glm::vec3(pos.x, pos.y, this->localPosition().z));
    });
    for (auto& behavior : _behaviors) {
      behavior->update(delta, this->shared_from_this());
    }
    setSleep(allSleeping);
  }
}

auto Node::getRoot() -> std::shared_ptr<Node> {
  if (_parent.isDefined()) {
    return _parent.get().getRoot();
  } else {
    return this->shared_from_this();
  }
}

auto Node::toBeDestroyed() -> bool {
  return _toBeDestroyed;
}
auto Node::markToBeDestroyed() -> void {
  _toBeDestroyed = true;
}

auto Node::setBoundingBox(BoundingBox box) -> void {
  _localBoundingBox = box;
  _boundingBox.setMax(box);
  if (_parent.isDefined()) {
    _parent.get()._addChildBB(box, localPosition());
  }
}

auto Node::boundingBox() -> BoundingBox {
  if (_shouldUpdate) {
    _update();
  }
  return _boundingBox;
}

auto Node::localBoundingBox() -> BoundingBox {
  return _localBoundingBox;
}
auto Node::_update() const -> void {
  _worldTransform = _parent.map([&](const auto& parentNode) {
    return parentNode.worldTransform() * _transform.matrix();
  }).getOrElse(_transform.matrix());
  _setUpdateFlag();
  _shouldUpdate = false;
}

auto Node::_addChildBB(BoundingBox box, glm::vec3 localPosition) -> void {
  auto bb = box;
  auto pos = localPosition;

  auto right = pos.x + bb.right();
  auto left = pos.x + bb.left();
  auto bottom = pos.y + bb.bottom();
  auto top = pos.y + bb.top();
  auto near = pos.y + bb.near();
  auto far = pos.y + bb.far();

  _boundingBox.setMaxRight(right);
  _boundingBox.setMaxLeft(left);
  _boundingBox.setMaxTop(top);
  _boundingBox.setMaxBottom(bottom);
  _boundingBox.setMaxNear(near);
  _boundingBox.setMaxFar(far);

  if (_parent.isDefined()) {
    _parent.get()._addChildBB(_boundingBox, this->localPosition());
  }
}

auto Node::_setUpdateFlag() const -> void {
  if (!_shouldUpdate) {
    _shouldUpdate = true;
    _children.values().foreach([](auto child) {
      child->_setUpdateFlag();
    });
  }
}

auto Node::_setParent(std::shared_ptr<Node> parent) -> void {
  _parent = Some(parent);
  this->_setUpdateFlag();
}
