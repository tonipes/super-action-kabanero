#pragma once

#include <tuple>
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "minebombers/behaviors/BulletBehaviour.hpp"

namespace NodeFactory {
  // Shapes need to be allocated in dynamic memory
  auto bombShape = new b2CircleShape;
  auto bulletShape = new b2CircleShape;

  int counter = 0;

  auto getId() -> int {
    counter++;
    return counter;
  }

  auto createBomb() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    bombShape->m_p.Set(0, 0);
    bombShape->m_radius = 0.1f;

    auto node = std::make_shared<Node<Transform3D>>("bomb_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/orb_of_destruction");
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.5f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = bombShape;
    fixtureDef->density = 5;
    fixtureDef->restitution = 0.3;

    node->addBehavior<BombBehaviour>(5.0f);

    node->addAttachment(sprite_att);
    node->addAttachment(material_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto createBullet() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    bulletShape->m_p.Set(0, 0);
    bulletShape->m_radius = 0.1f;

    auto node = std::make_shared<Node<Transform3D>>("bullet_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/sting0");
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = 10.0f;
    material_att->bulletRebound = true;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = bulletShape;
    fixtureDef->density = 1;
    fixtureDef->restitution = 1;

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.0f;
    bodyDef->bullet = true;

    node->addBehavior<BulletBehavior>(10.0f);

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }
};
