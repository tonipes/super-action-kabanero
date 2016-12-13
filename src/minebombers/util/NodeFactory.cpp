#include "minebombers/util/NodeFactory.hpp"

#include "minebombers/attachments/CollisionMaterialAttachment.hpp"
#include "minebombers/attachments/GunAttachment.hpp"
#include "minebombers/behaviors/BombBehaviour.hpp"
// #include "minebombers/hud/HudEffect.hpp"
#include "minebombers/behaviors/BulletBehaviour.hpp"
#include "minebombers/behaviors/RocketBehavior.hpp"
#include "minebombers/behaviors/HudBehavior.hpp"
#include "minebombers/behaviors/BulletOrientationBehavior.hpp"
#include "minebombers/behaviors/DamageAreaBehavior.hpp"
#include "minebombers/behaviors/MainMenuBehavior.hpp"
#include "minebombers/behaviors/BloodstainBehavior.hpp"
#include "minebombers/behaviors/MeatPieceBehavior.hpp"

#include "scene/attachment/SpriteAttachment.hpp"
#include "scene/attachment/EffectAttachment.hpp"
#include "graphics/effects/CircleEffect.hpp"
#include "graphics/effects/MainMenuEffect.hpp"

#include "minebombers/behaviors/EnemyBrainBehavior.hpp"
#include "minebombers/behaviors/EnemyGnomeBehavior.hpp"
#include "minebombers/behaviors/EnemyKnightBehavior.hpp"

#include "minebombers/level/LevelCompiler.hpp"

#include <glm/vec2.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_ITEM;
    fixtureDef->filter.maskBits = COLLISION_MASK_ITEM;

    node->addBehavior<BombBehaviour>(5.0f);
    node->setAllowSleep(false);

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

    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_BULLET;
    fixtureDef->filter.maskBits = COLLISION_MASK_BULLET;

    node->addBehavior<DamageAreaBehavior>(0.1f);

    node->addAttachment(material_att);
    node->setAllowSleep(false);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto _createRegularBullet(std::shared_ptr<GunParameters> params) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    auto node = std::make_shared<Node<Transform3D>>("bullet_" + std::to_string(getId()));
    node->setLocalPosition(glm::vec3(0, 0, 100));

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
    fixtureDef->restitution = 0;
    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_BULLET;
    fixtureDef->filter.maskBits = COLLISION_MASK_BULLET;

    node->addBehavior<BulletBehavior>(10.0f);
    node->addBehavior<BulletOrientationBehavior>();

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);
    node->setAllowSleep(false);

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
    fixtureDef->restitution = 0;

    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_BULLET;
    fixtureDef->filter.maskBits = COLLISION_MASK_BULLET;

    node->addBehavior<RocketBehavior>(params->explosionSize, params->damage);
    node->addBehavior<BulletOrientationBehavior>();

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);
    node->setAllowSleep(false);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto createMeatPieces(glm::vec2 loc, glm::vec2 dir, int count) ->
    std::vector<std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef>>>{
    auto random = Services::random();

    const float PI = 3.1415927;
    float sector = (PI / 3.0f) / count;
    auto direction = glm::rotate(dir, -sector * count / 2.0f);


    auto pieces = std::vector<std::tuple<std::shared_ptr<Node<Transform3D>>,std::shared_ptr<b2BodyDef>,std::shared_ptr<b2FixtureDef>>>();

    for(auto i = 0; i < count; i++){
      auto speed = 4.0f * random->nextFloat();
      auto rotatedDirection = glm::rotate(direction, sector * i * random->nextFloat());

      std::shared_ptr<Node<Transform3D>> node;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(node, bodyDef, fixtureDef) = NodeFactory::_createMeatPiece();

      bodyDef->position.Set(loc.x, loc.y);
      bodyDef->linearVelocity.Set(
        rotatedDirection.x * speed,
        rotatedDirection.y * speed
      );
      pieces.push_back(std::make_tuple(node, bodyDef, fixtureDef));

    }

    return pieces;
  }

  auto _createMeatPiece() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >{

    auto node = std::make_shared<Node<Transform3D>>("meat_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/blood_puddle_red1");
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = 0.0f;

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = false;
    bodyDef->linearDamping = 0.75f;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = 0.2f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 0.5;
    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_DECAL;
    fixtureDef->filter.maskBits = COLLISION_MASK_DECAL;
    // fixtureDef->isSensor = true;

    node->addBehavior<MeatPieceBehavior>(2.0f);

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);
    node->setAllowSleep(false);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto createBloodstain() ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> >{

    auto node = std::make_shared<Node<Transform3D>>("blood_" + std::to_string(getId()));

    auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/blood_red25");
    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = 0.0f;

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->fixedRotation = true;
    bodyDef->linearDamping = 0.0f;

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = 0.2f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 0.5;
    fixtureDef->isSensor = true;
    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_DECAL;
    fixtureDef->filter.maskBits = COLLISION_MASK_DECAL;

    node->addBehavior<BloodstainBehavior>(0.5f);

    node->addAttachment(material_att);
    node->addAttachment(sprite_att);
    node->setAllowSleep(false);

    return std::make_tuple(node, bodyDef, fixtureDef);
  }

  auto createMainMenu() -> std::shared_ptr<Node<Transform3D>> {
    std::vector<std::string> motds {
      "Tale of dungeons and guns",
      "It's like deathmatch nethack with guns!",
      "Try to find the ultimate weapon!",
      "Shoot everything that moves. Or doesn't move.",
      "If it moves, it hurts!"
    };

    auto motd = motds[Services::random()->nextInt(motds.size())];

    std::vector<std::string> c {
      "1 Player Survival     >>",
      "2 player Deathmatch  >>",
      "3 player Deathmatch  >>",
      "4 player Deathmatch  >>"
    };

    auto node = std::make_shared<Node<Transform3D>>("player1");
    node->addBehavior<MainMenuBehavior>(c, motd);

    auto effect_att = std::make_shared<EffectAttachment>(
      std::make_shared<MainMenuEffect>(c, motd)
    );

    node->addAttachment(effect_att);

    node->setAllowSleep(false);
    return node;
  }

  auto createRandomEnemy(float difficulty, int random) -> std::shared_ptr<Node<Transform3D>> {

    auto node = std::make_shared<Node<Transform3D>>("enemy_" + std::to_string(getId()));

    auto material_att = std::make_shared<CollisionMaterialAttachment>();

    material_att->collisionDamage = 1.0f * difficulty;
    material_att->isEnemy = true;

    node->addAttachment(material_att);
    node->setAllowSleep(false);

    if(random == 0) {
      auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/giant_orange_brain");
      node->addAttachment(sprite_att);
      node->addBehavior<EnemyBrainBehavior>(difficulty);

    } else if(random == 1) {
      auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/boggart");
      node->addAttachment(sprite_att);
      node->addBehavior<EnemyGnomeBehavior>(difficulty);

    } else if(random == 2) {
      auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/hell_knight");
      auto gun_att = std::make_shared<GunAttachment>(std::make_shared<GunParameters>(25.0f,  2.0f, 1, 0.1f, 10.0f, "tiles/pistol_normal", "test-effect/javelin0", "gunshot.ogg"));

      node->addAttachment(gun_att);
      node->addAttachment(sprite_att);

      node->addBehavior<EnemyKnightBehavior>(difficulty);

    }
    return node;
  }

  auto createPlayer(int playerId, int lives, glm::vec2 loc) ->
    std::tuple<
      std::shared_ptr<Node<Transform3D>>,
      std::shared_ptr<b2BodyDef>,
      std::shared_ptr<b2FixtureDef> > {

    auto name = "player" + std::to_string(playerId);
    auto cameraAddress = "world/camera" + std::to_string(playerId);

    auto node = std::make_shared<Node<Transform3D>>(name);
    node->setAllowSleep(false);
    // node->setLocalPosition(glm::vec3(loc.x, loc.y, 2));
    node->addAttachment(std::make_shared<SpriteAttachment>("tiles/spriggan_druid"));

    auto material_att = std::make_shared<CollisionMaterialAttachment>();
    material_att->isPlayer = true;
    node->addAttachment(material_att);

    auto gun = std::make_shared<GunAttachment>(
      std::make_shared<GunParameters>(150.0f,  2.0f, 1, 0.1f, 10.0f, "tiles/pistol_normal", "test-effect/crystal_spear0", "rocket_launch.ogg", true,  1.0f, 0, 0, "Rocket Launcher")
    );

    node->addAttachment(gun);

    node->addBehavior<PlayerBehaviour>(cameraAddress, playerId, lives);

    auto bodyDef = std::make_shared<b2BodyDef>();
    bodyDef->type = b2_dynamicBody;
    bodyDef->allowSleep = false;
    bodyDef->position.Set(loc.x, loc.y);

    auto shape = new b2CircleShape;
    shape->m_p.Set(0, 0);
    shape->m_radius = 0.35f;

    auto fixtureDef = std::make_shared<b2FixtureDef>();
    fixtureDef->shape = shape;
    fixtureDef->density = 1.0f;
    fixtureDef->filter.categoryBits = COLLISION_CATEGORY_PLAYER;
    fixtureDef->filter.maskBits = COLLISION_MASK_PLAYER;

    return std::make_tuple(node, bodyDef, fixtureDef);

  }

  // auto createFireball() ->
  //   std::tuple<
  //     std::shared_ptr<Node<Transform3D>>,
  //     std::shared_ptr<b2BodyDef>,
  //     std::shared_ptr<b2FixtureDef> > {
  //
  //     }
  //
  // auto createFireDecal() ->
  //   std::tuple<
  //     std::shared_ptr<Node<Transform3D>>,
  //     std::shared_ptr<b2BodyDef>,
  //     std::shared_ptr<b2FixtureDef> >{
  //
  //     }
}
