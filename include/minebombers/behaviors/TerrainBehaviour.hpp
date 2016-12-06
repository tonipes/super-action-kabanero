#pragma once

#include <iostream>
#include "message/event/DestroyNodeEvent.hpp"
#include "minebombers/level/TileMap.hpp"

class TerrainBehaviour : public Behavior<Transform3D> {
public:
  TerrainBehaviour(Node<Transform3D>* node, float health, std::shared_ptr<TileMap> map, int xc, int yc) :
   _maxHealth(health),
   _health(health),
   _node(node),
   _dmgToTake(0),
   _map(map),
   x(xc),
   y(yc)
  {}

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if (_dmgToTake != 0) {
      _health -= _dmgToTake;
      _dmgToTake = 0;
      std::cout << _health << "\n";
      if (_health <= 0) {
        Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(_node->path())));
        (*_map)[x][y].setType(OPEN_SIDE);
      }
    }
    node.setSleep(true);
  }

  auto takeDamage(float dmg) {
    _dmgToTake += dmg;
    _node->wakeRecursivelyUpwards();
  }

private:
  float _health, _maxHealth, _dmgToTake;
  std::shared_ptr<TileMap> _map;
  int x, y;
  Node<Transform3D>* _node;
};
