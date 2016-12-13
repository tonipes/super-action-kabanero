#pragma once

#include <tuple>
#include <Box2D/Box2D.h>
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"

#define COLLISION_CATEGORY_PLAYER 0x01
#define COLLISION_CATEGORY_ENEMY  0x02
#define COLLISION_CATEGORY_WALL   0x04
#define COLLISION_CATEGORY_PICKUP 0x08
#define COLLISION_CATEGORY_ITEM   0x10
#define COLLISION_CATEGORY_DECAL  0x20
#define COLLISION_CATEGORY_BULLET 0x30

// COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_WALL | COLLISION_CATEGORY_PICKUP | COLLISION_CATEGORY_ITEM | COLLISION_CATEGORY_DECAL | COLLISION_CATEGORY_BULLET
#define COLLISION_MASK_PLAYER COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_WALL | COLLISION_CATEGORY_PICKUP | COLLISION_CATEGORY_ITEM | COLLISION_CATEGORY_DECAL | COLLISION_CATEGORY_BULLET
#define COLLISION_MASK_ENEMY  COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_WALL | COLLISION_CATEGORY_BULLET
#define COLLISION_MASK_WALL   COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_WALL | COLLISION_CATEGORY_ITEM | COLLISION_CATEGORY_DECAL | COLLISION_CATEGORY_BULLET
#define COLLISION_MASK_PICKUP COLLISION_CATEGORY_PLAYER
#define COLLISION_MASK_ITEM   COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_WALL  | COLLISION_CATEGORY_ITEM | COLLISION_CATEGORY_BULLET
#define COLLISION_MASK_DECAL  COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_WALL
#define COLLISION_MASK_BULLET COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_WALL | COLLISION_CATEGORY_ITEM

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

  auto createRandomEnemy(float difficulty, int rand) -> std::shared_ptr<Node<Transform3D>> ;

  auto _createMeatPiece() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto createMeatPieces(glm::vec2 pos, glm::vec2 dir, int count) ->
    std::vector<std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef>>>;

  auto createBloodstain() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  auto createPlayer(int playerId, int lives, glm::vec2 loc) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >;

  // auto createFireball() ->
  //   std::tuple<
  //     std::shared_ptr<Node<Transform3D>>,
  //     std::shared_ptr<b2BodyDef>,
  //     std::shared_ptr<b2FixtureDef> >;
  //
  // auto createFireDecal() ->
  //   std::tuple<
  //     std::shared_ptr<Node<Transform3D>>,
  //     std::shared_ptr<b2BodyDef>,
  //     std::shared_ptr<b2FixtureDef> >;
};
