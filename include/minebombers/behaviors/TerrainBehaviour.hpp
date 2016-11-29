#pragma once

class TerrainBehaviour : public Behavior<Transform3D> {
public:
  TerrainBehaviour(Node<Transform3D>* node, float health, b2Body *physBody) : _maxHealth(health), _health(health), _physBody(physBody) {
  }

  auto update(float delta, Node<Transform3D>& node) -> void override {
  }

private:
  float _health, _maxHealth;
  b2Body *_physBody;
};
