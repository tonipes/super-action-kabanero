#pragma once

#include <iostream>
#include "message/event/DestroyNodeEvent.hpp"

class TerrainBehaviour : public Behavior<Transform3D> {
public:
  TerrainBehaviour(Node<Transform3D>* node, float health) : _maxHealth(health), _health(health), _node(node), _dmgToTake(0) {
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
    if (_dmgToTake != 0) {
      _health -= _dmgToTake;
      _dmgToTake = 0;
      std::cout << _health << "\n";
      if (_health <= 0) {
        Services::messagePublisher()->sendMessage(Message("game",std::make_shared<DestroyNodeEvent>(_node->path())));
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
  Node<Transform3D>* _node;
};
