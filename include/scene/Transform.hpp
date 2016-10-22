#pragma once

#include <iostream>

template <typename V, typename R, typename M>
class Transform {
public:

  Transform(V pos, R rot, V scale) : _position(pos), _rotation(rot), _scale(scale) {}

  auto matrix() const -> M {
    if (_recomputeTransform) {
      recomputeTransform();
      _recomputeTransform = false;
    }
    return _transform;
  }

  auto position() const -> V {
    return _position;
  }

  auto setPosition(V newPos) -> void {
    _position = newPos;
    _recomputeTransform = true;
  }

  auto rotation() const -> R {
    return _rotation;
  }

  auto setRotation(R rot) -> void {
    _rotation = rot;
    _recomputeTransform = true;
  }

  auto scale() const -> V {
    return _scale;
  }

  auto setScale(V scale) -> void {
    _scale = scale;
    _recomputeTransform = true;
  }

  typedef V vectorType;
  typedef R rotationType;
  typedef M matrixType;

protected:
  virtual auto recomputeTransform() const -> void = 0;

  mutable V _position;
  mutable R _rotation;
  mutable V _scale;
  mutable M _transform;
  mutable bool _recomputeTransform = true;
};

template <typename V, typename R, typename M>
std::ostream& operator<<(std::ostream& os, Transform<V, R, M> t) {
  os << "Transform" << std::endl <<
    "  position: " << t.position() << std::endl <<
    "  rotation: " << t.rotation() << std::endl <<
    "  scale:  " << t.scale();

  return os;
}
