#pragma once

#include "service/ResourceManager.hpp"
#include "resource/resource/Texture.hpp"
#include "scene/Scene.hpp"

/**
 * Renderer class.
 * Used to render scenes.
 * @todo Should we have seperate 2d and 3d renderers?
 * @todo Make actually render a scene
 */
class Renderer {
public:
  Renderer(sf::RenderWindow& window) : _window(window) {}
  ~Renderer() {}

  /**
   * Render scene with resources.
   * @param scene to draw.
   * @param resourceManager to get resources from.
   */
  template <typename T>
  auto render(Scene<T>& scene, ResourceManager& resourceManager) -> void {

    _window.clear(sf::Color::Black);

    const auto texture = resourceManager.get<Texture>("resources/textures/test_tileset.png").get().getTexture();
    sf::Sprite sprite(texture);
    _window.draw(sprite);

    _window.display();
  };

private:
  sf::RenderWindow& _window;
};
