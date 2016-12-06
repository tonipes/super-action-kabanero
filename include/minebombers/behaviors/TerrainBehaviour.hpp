#pragma once

#include <iostream>
// #include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/level/TileMap.hpp"
#include "scene/3D/Transform3D.hpp"
#include "game/Behavior.hpp"
#include "service/Services.hpp"
#include "message/event/DestroyNodeEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include "random/StdLibRandom.hpp"
#include "minebombers/attachments/GunAttachment.hpp"

class TerrainBehaviour : public Behavior<Transform3D> {
public:
  TerrainBehaviour(Node<Transform3D>* node, float health, std::shared_ptr<TileMap> map, int xc, int yc) :
   _maxHealth(health),
   _health(health),
   _node(node),
   _dmgToTake(0),
   _map(map),
   x(xc),
   y(yc){

    node->addEventReactor([&](CollisionEvent event) {
      if(event.collisionMaterialAttachment()->collisionDamage > 0.0f){
        this->takeDamage( event.collisionMaterialAttachment()->collisionDamage );
      }
    });
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if (_dmgToTake != 0) {
      _health -= _dmgToTake;
      _dmgToTake = 0;
      if (_health <= 0) {
        Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(_node->path())));
        (*_map)[x][y].setType(OPEN_SIDE);
      }
    }
    node.setSleep(true);
  }

  auto takeDamage(float dmg) -> void {
    _dmgToTake += dmg;
    _node->wakeRecursivelyUpwards();
  }

private:
  float _health, _maxHealth, _dmgToTake;
  std::shared_ptr<TileMap> _map;
  int x, y;
  Node<Transform3D>* _node;
};
