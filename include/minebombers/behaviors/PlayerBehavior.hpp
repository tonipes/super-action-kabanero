#pragma once

#include "minebombers/events/PlayerLocationEvent.hpp"
#include "minebombers/events/BulletEvent.hpp"
#include "minebombers/behaviors/BombBehaviour.hpp"
#include "minebombers/behaviors/EnemyOrbBehavior.hpp"
#include "message/event/CreateNodeEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "message/event/AudioClipEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include <Box2D/Box2D.h>
#include "random/StdLibRandom.hpp"
#include "minebombers/data/GunParameters.hpp"
#include "minebombers/util/NodeFactory.hpp"
#include "scene/attachment/PhysicsAttachment.hpp"
#include <cmath>

#include <glm/vec2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>


class PlayerBehavior : public Behavior {
public:
  PlayerBehavior(Node* node);

  const float PI = 3.1415927;

  auto update(float delta, std::shared_ptr<Node> node) -> void override;

private:
  int counter = 0;
  bool takeDamage = false;
  bool moveUp = false;
  bool moveRight = false;
  bool moveDown = false;
  bool moveLeft = false;

  bool fireUp = false;
  bool fireRight = false;
  bool fireDown = false;
  bool fireLeft = false;

  int _bulletsShot = 0;

  float _fireDelay = 0;

  bool throwBomb = false;

  float _playerSpeed = 3.0f;

  Option<std::shared_ptr<GunParameters>> _newGun = Option<std::shared_ptr<GunParameters>>();
};
