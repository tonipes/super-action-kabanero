#include "minebombers/behaviors/TerrainBehavior.hpp"

TerrainBehavior::TerrainBehavior(Node* node, float health, std::shared_ptr<TileMap> map) :
 _health(health),
 _map(map) {

  node->addEventReactor([&, node](CollisionEvent event) {
    auto material = event.collisionMaterialAttachment();

    if (material->collisionDamage > 0.0f) {
      const auto damage = event.collisionMaterialAttachment()->collisionDamage;
      _dmgToTake += damage;
      node->wakeUp();
    }
  });
}

auto TerrainBehavior::update(float delta, std::shared_ptr<Node> node) -> void {
  const auto& name = node->name().erase(0, 7);
  const auto& coords = split(name, '-');
  if (_dmgToTake > 0.0f) {
    _health -= _dmgToTake;
    _dmgToTake = 0;
    if (_health <= 0.0f) {
      Services::messagePublisher()->sendMessage(
        Message(
          "gameScene",
          std::make_shared<DestroyNodeEvent>(node->path())
        )
      );

      (*_map)[std::atoi(coords[0].c_str())][std::atoi(coords[1].c_str())].setType(OPEN_SIDE);
    }
  }
  node->setSleep(true);
}
