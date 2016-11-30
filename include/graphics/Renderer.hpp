#pragma once

#include "service/ResourceManager.hpp"
#include "resource/resource/Texture.hpp"
#include "resource/resource/sprite/Atlas.hpp"
#include "resource/resource/sprite/Sprite.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneView.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/3D/Transform3D.hpp"
#include "scene/2D/Transform2D.hpp"
#include "graphics/SpriteBatch.hpp"
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
    _tilesize(tilesize),
    _spriteBatch(SpriteBatch(tilesize)) {}

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
    const auto& atlas = *Services::resourceManager()->getRequired<Atlas>("resources/atlases/pack.atlas");

    _window.clear(sf::Color::Black);

    auto cameraNode  = sceneView.cameraNode();
    auto rootNode = sceneView.rootNode();
    auto viewport = sceneView.viewport();

    // auto nodes = _getAllNodes(rootNode);

    // for (auto& node : nodes) {
      _renderNode(rootNode, cameraNode, atlas);
    // }

    // _renderNode(rootNode, cameraNode);
    // _spriteBatch.flush(_window);
    _window.display();
  };

private:
  sf::RenderWindow& _window;
  int _tilesize;
  SpriteBatch _spriteBatch;
  KBMap<std::string, sf::Texture> textures;

  auto _getAllNodes(std::shared_ptr<Node<Transform3D>> node) -> KBVector<std::shared_ptr<Node<Transform3D>>> {
    auto v = KBVector<std::shared_ptr<Node<Transform3D>>>();
    v += node;
    auto children = node->children().values();
    children.foreach([&](auto child) {
      auto n = _getAllNodes(child);
      for (auto c : n) {
        v += c;
      }
    });
    return v;
  }

  auto _renderNode(const std::shared_ptr<Node<Transform3D>> node, const std::shared_ptr<Node<Transform3D>> cameraNode, const Atlas& atlas) -> void {
    const auto& resourceManager = Services::resourceManager();
    auto spriteAttachment = node->get<SpriteAttachment>();

    spriteAttachment.foreach([&](auto s) {
      auto id = s.spriteId();
      const auto& someSprite = atlas.get(id);
      if (someSprite.isDefined()) {
        auto sprite = someSprite.get();
        auto texPath = sprite.texturePath();
        auto xy = sprite.xy();
        auto spriteSize = sprite.size();

        if (!textures.contains(texPath)) {
          textures.insert(texPath, resourceManager->getRequired<Texture>(texPath)->getTexture());
        }
        const auto& tex =textures[texPath];
        sf::Sprite sfSprite(tex);
        sfSprite.setTextureRect(sf::IntRect(
          xy.x,
          xy.y,
          spriteSize.x,
          spriteSize.y
        ));

        const auto& nodePosition = node->position();
        const auto& camPosition = cameraNode->position();
        const auto& windowSize = _window.getSize();

        auto relativePosition = (nodePosition - camPosition) * (float)_tilesize;

        sfSprite.move(relativePosition.x + windowSize.x / 2, -relativePosition.y + windowSize.y / 2);

        _window.draw(sfSprite);
      } else {
        throw ResourceException("Atlas does not contain sprite with id: " + id);
      }
    });


    auto children = node->children();

    for (auto child : children) {
      std::string name;
      std::shared_ptr<Node<Transform3D>> node;
      std::tie(name, node) = child;
      _renderNode(node, cameraNode, atlas);
    }
  }

};
