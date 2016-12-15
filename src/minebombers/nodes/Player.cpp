#include "minebombers/nodes/Player.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"


auto Player::create(
    std::string playerId,
    b2World& world) -> std::shared_ptr<Node> {

  auto node = std::make_shared<Node>(playerId);
  node->setAllowSleep(false);
  node->setLocalPosition(glm::vec3(0, 0, 2));

  auto sprite = std::make_shared<SpriteAttachment>("player/survivor-idle_rifle_0-small");

  node->addAttachment(sprite);

  auto material_att = std::make_shared<CollisionMaterialAttachment>();
  material_att->isPlayer = true;

  node->addAttachment(material_att);
  // auto gun = std::make_shared<GunAttachment>(
  //   normalGuns[0]
  // );

  // node->addAttachment(gun);

  auto physCircle = createPhysCircle(world);
  auto physAttachment = std::make_shared<PhysicsAttachment>(physCircle);

  node->addAttachment(physAttachment);

  node->addBehavior<PlayerBehavior>();

  return node;
}

auto Player::createPhysCircle(b2World& world) -> b2Body* {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.allowSleep = false;
  b2Body* body = world.CreateBody(&bodyDef);
  b2CircleShape circleShape;
  circleShape.m_p.Set(0, 0); //position, relative to body position
  circleShape.m_radius = 0.35f;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &circleShape;
  // fixtureDef.filter.groupIndex = -1;
  fixtureDef.density = 1.0f;
  body->CreateFixture(&fixtureDef);
  return body;
}
