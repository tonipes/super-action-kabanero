#pragma once

class TerrainBehaviour : public Behavior<Transform3D> {
public:
  TerrainBehaviour(Node<Transform3D>* node, float health) : _maxHealth(health), _health(health){
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
  }

private:
  float _health, _maxHealth;
};
