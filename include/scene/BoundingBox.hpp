#pragma once

#include <glm/vec3.hpp>

class BoundingBox {
public:
  BoundingBox() {}

  BoundingBox(glm::vec3 leftBottomNear, glm::vec3 rightTopFar) :
      _left(leftBottomNear.x),
      _bottom(leftBottomNear.y),
      _near(leftBottomNear.z),
      _right(rightTopFar.x),
      _top(rightTopFar.y),
      _far(rightTopFar.z) {}

  auto left() const { return _left; }
  auto right() const { return _right; }
  auto bottom() const { return _bottom; }
  auto top() const { return _top; }
  auto near() const { return _near; }
  auto far() const { return _far; }

  auto setMax(BoundingBox box) -> void {
    setMaxRight(box.right());
    setMaxLeft(box.left());
    setMaxBottom(box.bottom());
    setMaxTop(box.top());
    setMaxNear(box.near());
    setMaxFar(box.far());
  }

  auto setMaxRight(float right) -> void {
    if (right > _right) {
      _right = right;
    }
  }

  auto setMaxLeft(float left) -> void {
    if (left < _left) {
      _left = left;
    }
  }

  auto setMaxBottom(float bottom) -> void {
    if (bottom < _bottom) {
      _bottom = bottom;
    }
  }

  auto setMaxTop(float top) -> void {
    if (top > _top) {
      _top = top;
    }
  }

  auto setMaxNear(float near) -> void {
    if (near > _near) {
      _near = near;
    }
  }

  auto setMaxFar(float far) -> void {
    if (far < _far) {
      _far = far;
    }
  }

private:
  float _left = 0.0f;
  float _right = 0.0f;
  float _bottom = 0.0f;
  float _top = 0.0f;
  float _near = 0.0f;
  float _far = 0.0f;
};
