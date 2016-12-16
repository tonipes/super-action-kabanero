#pragma once

#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "message/event/CollisionEvent.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "minebombers/util/NodeFactory.hpp"

#include <glm/vec2.hpp>

#include <iostream>

class EnemyBehavior : public Behavior<Transform3D> {
public:
  EnemyBehavior(Node<Transform3D>* node, float difficulty) : _difficulty(difficulty){
    moveDirection.x = 0.0f;
    moveDirection.y = 0.0f;

    node->addEventReactor([&](CollisionEvent event) {
      if(event.collisionMaterialAttachment()->collisionDamage > 0.0f && !event.collisionMaterialAttachment()->isEnemy) {
        _dmgToTake += event.collisionMaterialAttachment()->collisionDamage;
        _collisionVec = event.vec();
      }
      _collided = true;
    });

  }

  virtual auto update(float delta, Node<Transform3D>& node) -> void override {

  }

protected:
  auto updateCommon(float delta, Node<Transform3D>& node) -> void {
    auto pos = node.position().xy();

    const auto& physAttachment = node.get<PhysicsAttachment>();
    physAttachment.foreach([&](auto phys) {
      phys.setVelocity(moveDirection.x, moveDirection.y);
    });

    if (_dmgToTake > 0.0f) {
      _health -= _dmgToTake;

      auto myPos = node.position();

      auto damageParticle = NodeFactory::createDamageParticle((int)_dmgToTake);
      auto randomPos = (Services::random()->nextFloat() - 0.5f) * 1.0f;

      damageParticle->setLocalPosition(glm::vec3(myPos.x + randomPos, myPos.y, myPos.z + 1));

      Services::messagePublisher()->sendMessage(
        Message("gameScene",
          std::make_shared<CreateNodeEvent>("world",
            damageParticle, nullptr, nullptr
          )
        )
      );

      _dmgToTake = 0;

      if (_health <= 0.0f) {

        Services::messagePublisher()->sendMessage(
          Message(
            "gameScene",
            std::make_shared<DestroyNodeEvent>(node.path())
          )
        );

        auto meat = NodeFactory::createMeatPieces(pos, glm::normalize(_collisionVec),  6);

        for(auto m : meat){
          Services::messagePublisher()->sendMessage(Message("gameScene", std::make_shared<CreateNodeEvent>(
            "world/bullets", std::get<0>(m), std::get<1>(m), std::get<2>(m)
          )));
        }

      }
    }
  }

  float _dmgToTake = 0.0f;
  glm::vec2 moveDirection;
  float _difficulty;
  float _health = 0.0f;
  bool _collided = false;
  glm::vec2 _collisionVec = glm::vec2(0,0);

};
