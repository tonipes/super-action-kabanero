#pragma once

class Viewport {
public:
  Viewport(
      float x,
      float y,
      float w,
      float h) : _x(x), _y(y), _w(w), _h(h) {}

  auto x() const -> float { return _x; }
  auto y() const -> float { return _y; }
  auto w() const -> float { return _w; }
  auto h() const -> float { return _h; }

private:
  float _x;
  float _y;
  float _w;
  float _h;
};
