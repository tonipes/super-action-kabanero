#pragma once

#include "service/ResourceManager.hpp"
#include "resource/resource/Texture.hpp"
#include "resource/resource/sprite/Atlas.hpp"
#include "resource/resource/sprite/Sprite.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneView.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "scene/3D/Transform3D.hpp"
#include "scene/2D/Transform2D.hpp"
#include "graphics/SpriteBatch.hpp"
#include "graphics/Effect.hpp"
#include "service/Services.hpp"
#include "minebombers/attachments/VisibilityAttachment.hpp"


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

  const int WINDOW_WIDTH = 32 * 48;
  const int WINDOW_HEIGHT = 32 * 30;

  ~Renderer() {}

  auto render(const KBVector<SceneView>& sceneViews) -> void {
    _window.setView(_window.getDefaultView());
    _window.clear(sf::Color::Black);
    for (const auto& sceneView : sceneViews) {
      // std::cout << "x: " << sceneView.cameraNode()->position().x << std::endl;
      // _window.setView(_window.getDefaultView());
      render(sceneView);
    }
    _window.display();
  }

  sf::View view;
  /**
   * Render scene with resources.
   * @param scene to draw.
   * @param resourceManager to get resources from.
   */
  auto render(const SceneView& sceneView) -> void {
    auto resourceManager = Services::resourceManager();
    const auto& atlas = *Services::resourceManager()->getRequired<Atlas>("resources/atlases/pack.atlas");

    const auto viewport = sceneView.viewport();
    const auto& windowSize = _window.getSize();

    auto cameraNode  = sceneView.cameraNode();
    auto rootNode = sceneView.rootNode();
    this->_cameraPosition = cameraNode->position();

    _viewportSize.x = WINDOW_WIDTH * viewport.w() / _tilesize * 0.5f;
    _viewportSize.y = WINDOW_HEIGHT * viewport.h() / _tilesize * 0.5f;
    _viewportOffset.x = WINDOW_WIDTH * viewport.x() / _tilesize;
    _viewportOffset.y = WINDOW_HEIGHT * viewport.y() / _tilesize;
    leftEdge = -_viewportSize.x + _cameraPosition.x - 1;
    rightEdge = _viewportSize.x + _cameraPosition.x + 1;
    topEdge = _viewportSize.y + _cameraPosition.y + 1;
    bottomEdge = -_viewportSize.y + _cameraPosition.y - 1;

    auto ratio = (windowSize.x / (float)WINDOW_WIDTH) / (windowSize.y / (float)WINDOW_HEIGHT);
    auto shift = (viewport.w() - (viewport.w() / ratio)) / 2;

    view.reset(
      sf::FloatRect(
        WINDOW_WIDTH * viewport.x(),
        WINDOW_HEIGHT * viewport.y(),
        WINDOW_WIDTH * viewport.w(),
        WINDOW_HEIGHT * viewport.h()));
    view.setViewport(sf::FloatRect(viewport.x() + shift, viewport.y(), viewport.w() / ratio, viewport.h()));
    _window.setView(view);

    this->_windowSize = _window.getSize();

    _renderNode(rootNode, cameraNode, atlas);

    for (auto& kv : sprites) {
      for (auto& sprite : kv.second) {
        _window.draw(sprite);
      }
    }
    sprites = std::map<int, std::vector<sf::Sprite>>();
    for (auto& effect : effects) {
      _window.draw(*effect.get());
    }
    effects = std::vector<std::shared_ptr<Effect>>();
  }

private:
  sf::RenderWindow& _window;
  glm::vec2 _viewportSize;
  glm::vec2 _viewportOffset;
  float _tilesize;
  SpriteBatch _spriteBatch;
  KBMap<std::string, sf::Texture> textures;
  glm::vec3 _cameraPosition;
  sf::Vector2<uint> _windowSize;
  float leftEdge;
  float rightEdge;
  float topEdge;
  float bottomEdge;

  std::map<int, std::vector<sf::Sprite>> sprites;
  std::vector<std::shared_ptr<Effect>> effects;

  auto _isWithinWindow(const glm::vec3& nodePosition, const BoundingBox& boundingBox) -> bool;

  auto _renderNode(const std::shared_ptr<Node> node, const std::shared_ptr<Node> cameraNode, const Atlas& atlas) -> void {
    const auto& boundingBox = node->boundingBox();

    const auto& nodePosition = node->position();

    if (_isWithinWindow(nodePosition, boundingBox)) {
      bool isVisible = cameraNode->get<VisibilityAttachment>().get().isVisible(
        round(nodePosition.x),
        round(nodePosition.y)
      );

      const auto relativePosition = nodePosition - _cameraPosition;

      if (isVisible) {
        const auto& spriteAttachment = node->get<SpriteAttachment>();
        if (spriteAttachment.isDefined()) {
          const auto& s = spriteAttachment.get();
          const auto& id = s.spriteId();

          const auto& someSprite = atlas.get(id);
          if (someSprite.isDefined()) {
            const auto& sprite = someSprite.get();
            const auto& texPath = sprite.texturePath();
            const auto& xy = sprite.xy();
            const auto& spriteSize = sprite.size();

            if (!textures.contains(texPath)) {
              const auto& resourceManager = Services::resourceManager();
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

            const auto& q = node->rotation();
            auto rot = glm::degrees(glm::atan(
              2.0f * (q.x * q.y + q.z * q.w),
              q.x * q.x - q.y * q.y - q.z * q.z + q.w * q.w)
            );

            sfSprite.setOrigin(spriteSize.x / 2.0f, spriteSize.y / 2.0f);
            sfSprite.rotate(rot);

            sfSprite.move(
              (relativePosition.x + _viewportSize.x + _viewportOffset.x) * _tilesize,
              (-relativePosition.y + _viewportSize.y + _viewportOffset.y) * _tilesize);


            sprites[relativePosition.z].push_back(std::move(sfSprite));
          } else {
            throw ResourceException("Atlas does not contain sprite with id: " + id);
          }
        }
      }

      const auto& effectAttachment = node->get<EffectAttachment>();

      effectAttachment.foreach([&](auto e) {
        auto effect = e.effect();

        effect->setPosition(
          (relativePosition.x + _viewportSize.x + _viewportOffset.x) * _tilesize,
          (-relativePosition.y + _viewportSize.y + _viewportOffset.y) * _tilesize
        );

        effect->setTileSize(_tilesize);

        effects.push_back(effect);
      });

      const auto& children = node->children();

      for (const auto& child : children) {
        _renderNode(std::get<1>(child), cameraNode, atlas);
      }
    }
  }

};
