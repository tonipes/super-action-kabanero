#pragma once

#include "resource/ResourceManager.hpp"
#include "resource/resource/Texture.hpp"
#include "scene/Scene.hpp"

/**
 * Renderer class.
 * Used to render scenes.
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
  auto render(Scene& scene, ResourceManager& resourceManager) -> void {
    // std::cout << "Rendering! " << std::endl;

    _window.clear(sf::Color::Black);

    // TODO: This is just for testing
    auto textureResource = resourceManager.get<Texture>("resources/textures/test_tileset.png");
    if(textureResource.isDefined()){
      auto got = textureResource.get();
      auto texture = got.getTexture();
      sf::Sprite sprite(texture);
      _window.draw(sprite);
    }

    _window.display();
  };

private:
  sf::RenderWindow& _window;
};
