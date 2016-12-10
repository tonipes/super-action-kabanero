#pragma once

#include <tuple>
#include <Box2D/Box2D.h>
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"

namespace NodeFactory {
  extern int counter;

  auto getId() -> int;

  auto createBomb() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto createBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto _createRegularBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto _createRocketBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto createDamageCircle(float radius, float damage, float force) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto createMainMenu() -> std::shared_ptr<Node<Transform3D>>;

};
