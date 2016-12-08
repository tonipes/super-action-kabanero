#include "minebombers/util/NodeFactory.hpp"

#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "minebombers/behaviors/BombBehaviour.hpp"
#include "minebombers/hud/HudEffect.hpp"
#include "minebombers/behaviors/BulletBehaviour.hpp"
#include "minebombers/behaviors/RocketBehavior.hpp"
#include "minebombers/behaviors/HudBehavior.hpp"
#include "minebombers/behaviors/BulletOrientationBehavior.hpp"
#include "minebombers/behaviors/DamageAreaBehavior.hpp"
#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "graphics/effects/CircleEffect.hpp"
#include "minebombers/data/HudParameters.hpp"

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
    shape->m_radius = 0.4f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 5;
    fixtureDef->restitution = 0.3;

    node->addBehavior<BombBehaviour>(5.0f);

    node->addAttachment(sprite_att);
    node->addAttachment(material_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto createBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    if(params->isRocketLauncher) return _createRocketBullet(params);
    else return _createRegularBullet(params);

  }

  auto createDamageCircle(float radius, float damage, float force = 0.0f) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    auto node = std::make_shared<Node<Transform3D>>("damageCircle_" + std::to_string(getId()));

    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = damage;
    material_att->force = force;

    auto effect_att = std::make_shared<EffectAttachment>(std::make_shared<CircleEffect>(radius));
    node->addAttachment(effect_att);

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = radius;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->isSensor = true;

    node->addBehavior<DamageAreaBehavior>(0.1f);

    node->addAttachment(material_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto _createRegularBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    auto node = std::make_shared<Node<Transform3D>>("bullet_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>(params->bulletSprite);
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = params->damage;
    material_att->bulletRebound = true;

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.0f;
    bodyDef->bullet = true;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = params->bulletSize;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 1;
    fixtureDef->restitution = 1;

    node->addBehavior<BulletBehavior>(10.0f);
    node->addBehavior<BulletOrientationBehavior>();

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto _createRocketBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    auto node = std::make_shared<Node<Transform3D>>("rocket_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>(params->bulletSprite);
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = 0.0f; // Rockets don't hurt. Explosions do

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.0f;
    bodyDef->bullet = true;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = params->bulletSize;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 1;
    fixtureDef->restitution = 1;

    node->addBehavior<RocketBehavior>(params->explosionSize, params->damage);
    node->addBehavior<BulletOrientationBehavior>();

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto createHud(std::string name, std::string hudName) -> std::shared_ptr<Node<Transform3D>> {
    auto node = std::make_shared<Node<Transform3D>>(name);
    node->setLocalPosition(glm::vec3(0.0f, 0.0f, 100.0f));

    auto hud_att = std::make_shared<EffectAttachment>(std::make_shared<HudEffect>(0,""));
    node->addAttachment(hud_att);

    node->addBehavior<HudBehavior>();

    return node;
  }
}
