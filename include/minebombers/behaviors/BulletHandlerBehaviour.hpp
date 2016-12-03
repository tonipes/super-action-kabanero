#pragma once

#include "minebombers/events/BulletEvent.hpp"
#include "minebombers/behaviors/BulletBehaviour.hpp"

#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class BulletHandlerBehaviour : public Behavior<Transform3D> {
public:
  BulletHandlerBehaviour(Node<Transform3D>* node, b2World& physWorld) : _physWorld(physWorld) {
    node->addEventReactor([&](BulletEvent event) {
      bulletEvents += event;
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    for (auto e : bulletEvents){
      if (e.getAction() == CREATE_BULLET){
        auto x = e.getX();
        auto y = e.getY();
        auto vx = e.getVX();
        auto vy = e.getVY();
        auto speed = e.getSpeed();
        auto name = getName();

        // Calculate real spawn position. You don't want the bullet to spawn inside the player
        auto real_x = x + vx * 0.2f;
        auto real_y = y + vy * 0.2f;

        // Services::logger()->debug("Create bullet at " + std::to_string(x) + ", " + std::to_string(y));

        auto bulletNode = std::make_shared<Node<Transform3D>>(name);
        bulletNode->setLocalPosition(glm::vec3(real_x, real_y, 5));

        auto sprite_att = std::make_shared<SpriteAttachment>("test-effect/bolt01");
        auto physBody = createPhysCircle(real_x, real_y);

        physBody->SetLinearVelocity(b2Vec2(vx * speed, vy * speed));

        bulletNode->addBehavior<BulletBehavior>(physBody, 10.0f);
        bulletNode->addAttachment(sprite_att);

        node.addChild(bulletNode);

        Services::logger()->debug("Create bullet:  " + bulletNode->name() );

      } else if (e.getAction() == DESTROY_BULLET) {
        Services::logger()->debug("destroy bullet");
      }

    }
    bulletEvents = KBVector<BulletEvent>();
  }



private:
  KBVector<BulletEvent> bulletEvents;

  int counter = 0; // How to get names O.o?
  b2World& _physWorld;

  auto getName() -> std::string {
    counter++;
    return "bullet_" + std::to_string(counter);
  }

  auto createPhysCircle(float x, float y) -> b2Body* {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.allowSleep = false;
    bodyDef.fixedRotation = true;
    bodyDef.linearDamping = 0.0f;
    bodyDef.bullet = true;

    b2Body* body = _physWorld.CreateBody(&bodyDef);
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = 0.1f;
    body->CreateFixture(&circleShape, 1.0f);
    return body;
  }
};
