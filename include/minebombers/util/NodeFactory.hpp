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

  auto createBullet() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

};
