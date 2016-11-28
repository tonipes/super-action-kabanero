#pragma once

#include "resource/resource/sprite/Sprite.hpp"
#include "collection/mutable/KBMap.hpp"
#include "scene/2D/Transform2D.hpp"


class SpriteBatch {
public:
  SpriteBatch(int tileSize) : _tileSize(tileSize) { }

  auto draw(
      Sprite sprite,
      Transform2D transform,
      glm::vec2 camPosition,
      glm::vec2 windowSize) -> void {

    auto texSize = sprite.texSize();
    auto spriteSize = sprite.size();

    auto mat = transform.matrix();
    auto windowShift = windowSize / 2.0f;
    auto totalShift = -camPosition * (float)_tileSize + windowShift;

    auto xy1 =
      glm::vec2(mat * glm::vec3(-spriteSize.x / 2.0f, -spriteSize.y / 2.0f, 1))
      * (float)_tileSize + totalShift;
    auto xy2 =
      glm::vec2(mat * glm::vec3(-spriteSize.x / 2.0f, spriteSize.y / 2.0f, 1))
      * (float)_tileSize + totalShift;
    auto xy3 =
      glm::vec2(mat * glm::vec3(spriteSize.x / 2.0f, spriteSize.y / 2.0f, 1))
      * (float)_tileSize + totalShift;
    auto xy4 =
      glm::vec2(mat * glm::vec3(-spriteSize.x / 2.0f, -spriteSize.y / 2.0f, 1))
      * (float)_tileSize + totalShift;

    if (
      _checkInBounds(xy1, windowSize) &&
      _checkInBounds(xy2, windowSize) &&
      _checkInBounds(xy3, windowSize) &&
      _checkInBounds(xy4, windowSize)) {

      auto texPath = sprite.texturePath();
      auto st = sprite.xy() / texSize;
      auto uv = (sprite.xy() + spriteSize) / texSize;

      _addVertex(texPath, xy1, st.x, st.y);
      _addVertex(texPath, xy2, st.x, uv.y);
      _addVertex(texPath, xy3, uv.x, uv.y);
      _addVertex(texPath, xy4, uv.x, st.y);
    }
  }

  auto flush() -> void {
    
  }
private:
  auto _checkInBounds(glm::vec2 point, glm::vec2 bounds) -> bool {
    return point.x > 0 && point.x < bounds.x && point.y > 0 && point.y < bounds.y;
  }

  auto _addVertex(std::string texPath, glm::vec2 xy, float u, float v) -> void {
    if (!vertices.contains(texPath)) {
      vertices[texPath] = KBVector<float>();
    }
    vertices[texPath] += xy.x;
    vertices[texPath] += xy.y;
    vertices[texPath] += u;
    vertices[texPath] += v;
  }

  int _tileSize;

  // Texture id as key
  KBMap<std::string, KBVector<float>> vertices;

};
