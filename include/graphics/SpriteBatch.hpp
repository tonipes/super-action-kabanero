#pragma once

#include "resource/resource/sprite/Sprite.hpp"
#include "collection/mutable/KBMap.hpp"
#include "scene/2D/Transform2D.hpp"
#include <SFML/Graphics.hpp>


class SpriteBatch {
public:
  SpriteBatch(int tileSize) : _tileSize(tileSize) { }

  auto draw(
      Sprite sprite,
      glm::vec2 position) -> void {

    const auto& texSize = sprite.texSize();
    const auto& spriteSize = sprite.size();

    auto halfWidth = spriteSize.x / 2.0f;
    auto halfHeight = spriteSize.y / 2.0f;

    // auto mat = transform.matrix();

    // auto xy1 =
    //   glm::vec2(mat * glm::vec3(-spriteSize.x / 2.0f, -spriteSize.y / 2.0f, 1))
    //   * (float)_tileSize;
    // auto xy2 =
    //   glm::vec2(mat * glm::vec3(-spriteSize.x / 2.0f, spriteSize.y / 2.0f, 1))
    //   * (float)_tileSize;
    // auto xy3 =
    //   glm::vec2(mat * glm::vec3(spriteSize.x / 2.0f, spriteSize.y / 2.0f, 1))
    //   * (float)_tileSize;
    // auto xy4 =
    //   glm::vec2(mat * glm::vec3(-spriteSize.x / 2.0f, -spriteSize.y / 2.0f, 1))
    //   * (float)_tileSize;



    const auto xy1 = glm::vec2(-halfWidth, -halfHeight) + position;
    const auto xy2 = glm::vec2(-halfWidth, halfHeight) + position;
    const auto xy3 = glm::vec2(halfWidth, halfHeight) + position;
    const auto xy4 = glm::vec2(halfWidth, -halfHeight) + position;


    const auto texPath = sprite.texturePath();
    const auto st = sprite.xy();
    const auto uv = (sprite.xy() + spriteSize);

    _addVertex(texPath, xy1, st.x, st.y);
    _addVertex(texPath, xy2, st.x, uv.y);
    _addVertex(texPath, xy3, uv.x, uv.y);
    _addVertex(texPath, xy4, uv.x, st.y);
  }

  auto flush(sf::RenderWindow& window) -> void {
    for (auto& vertArray : vertices.values()) {
      window.draw(&vertArray[0], vertArray.size(), sf::Quads);
    }
    vertices = KBMap<std::string, std::vector<sf::Vertex>>();
  }
private:
  auto _checkInBounds(glm::vec2 point, glm::vec2 bounds) -> bool {
    return point.x > 0 && point.x < bounds.x && point.y > 0 && point.y < bounds.y;
  }

  auto _addVertex(std::string texPath, glm::vec2 xy, float u, float v) -> void {
    if (!vertices.contains(texPath)) {
      vertices[texPath] = std::vector<sf::Vertex>();
    }
    sf::Vertex vertex(sf::Vector2f(xy.x, xy.y), sf::Color::Red, sf::Vector2f(u, v));
    vertices[texPath].push_back(vertex);
    // vertices[texPath] += xy.x;
    // vertices[texPath] += xy.y;
    // vertices[texPath] += u;
    // vertices[texPath] += v;
  }

  int _tileSize;

  // Texture id as key
  KBMap<std::string, std::vector<sf::Vertex>> vertices;

};
