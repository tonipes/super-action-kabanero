#pragma once

class Viewport {
public:
  Viewport(
      float x,
      float y,
      float w,
      float h) : _x(x), _y(y), _w(w), _h(h) {}

  auto x() -> float { return _x; }
  auto y() -> float { return _y; }
  auto w() -> float { return _w; }
  auto h() -> float { return _h; }

private:
  float _x;
  float _y;
  float _w;
  float _h;
};
