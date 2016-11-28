#pragma once

#include "service/ResourceManager.hpp"
#include "resource/resource/Texture.hpp"
#include "resource/resource/sprite/Atlas.hpp"
#include "resource/resource/sprite/Sprite.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneView.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

/**
 * Renderer class.
 * Used to render scenes.
 * @todo Should we have seperate 2d and 3d renderers?
 * @todo Make actually render a scene
 */
class Renderer {
public:
  Renderer(sf::RenderWindow& window, int tilesize) :
    _window(window),
    _tilesize(tilesize) {}

  ~Renderer() {}

  auto render(KBVector<SceneView<Transform3D>> sceneViews) -> void {
    sceneViews.foreach([&](auto sceneView) {
      render(sceneView);
    });
  }

  /**
   * Render scene with resources.
   * @param scene to draw.
   * @param resourceManager to get resources from.
   */
  auto render(SceneView<Transform3D>& sceneView) -> void {
    auto resourceManager = Services::resourceManager();

    _window.clear(sf::Color::Black);

    auto cameraNode  = sceneView.cameraNode();
    auto rootNode = sceneView.rootNode();
    auto viewport = sceneView.viewport();

    _renderNode(rootNode, cameraNode);

    _window.display();
  };

private:
  sf::RenderWindow& _window;
  int _tilesize;

  auto _renderNode(const std::shared_ptr<Node<Transform3D>> node, const std::shared_ptr<Node<Transform3D>> cameraNode) -> void {
    auto resourceManager = Services::resourceManager();
    auto atlas = resourceManager->getRequired<Atlas>("resources/atlases/pack.atlas");

    auto spriteAttachment = node->get<SpriteAttachment>();

    spriteAttachment.foreach([&](auto s) {
      auto id = s.spriteId();
      auto someSprite = atlas->get(id);
      if (someSprite.isDefined()) {
        auto sprite = someSprite.get();
        auto texPath = sprite.texturePath();
        const auto tex = resourceManager->getRequired<Texture>(texPath)->getTexture();
        auto texSize = tex.getSize();
        auto xy = sprite.xy();
        auto spriteSize = sprite.size();

        sf::Sprite sfSprite(tex);
        sfSprite.setTextureRect(sf::IntRect(
          xy.x,
          xy.y,
          spriteSize.x,
          spriteSize.y
        ));

        auto nodePosition = node->position();
        auto camPosition = cameraNode->position();
        auto windowSize = _window.getSize();

        auto relativePosition = (nodePosition - camPosition) * (float)_tilesize;

        sfSprite.move(relativePosition.x + windowSize.x / 2, -relativePosition.y + windowSize.y / 2);

        _window.draw(sfSprite);
      } else {
        throw ResourceException("Atlas does not contain sprite with id: " + id);
      }
    });


    auto children = node->children();

    children.values().foreach([&](const auto& child) {
      _renderNode(child, cameraNode);
    });
  }

};
