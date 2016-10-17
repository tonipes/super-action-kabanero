#pragma once

template <typename V, typename R, typename M>
class Transform {
public:

  Transform(V pos, R rot, V scale) : _position(pos), _rotation(rot), _scale(scale) {}

  auto matrix() -> M {
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
protected:
  virtual void recomputeTransform() = 0;

  V _position;
  R _rotation;
  V _scale;
  M _transform;
  bool _recomputeTransform = true;
};
