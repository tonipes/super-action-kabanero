#include "minebombers/util/NodeFactory.hpp"

#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "minebombers/behaviors/BombBehaviour.hpp"
#include "minebombers/behaviors/BulletBehaviour.hpp"
#include "scene/attachment/SpriteAttachment.hpp"

namespace NodeFactory {
  int counter = 0;

  auto getId() -> int {
    NodeFactory::counter++;
    return NodeFactory::counter;
  }

  auto createBomb() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    auto node = std::make_shared<Node<Transform3D>>("bomb_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/orb_of_destruction");
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.5f;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = 0.1f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
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

    auto node = std::make_shared<Node<Transform3D>>("bullet_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/sting0");
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = 10.0f;
    material_att->bulletRebound = true;

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.0f;
    bodyDef->bullet = true;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = 0.1f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 1;
    fixtureDef->restitution = 1;

    node->addBehavior<BulletBehavior>(10.0f);

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }
}
