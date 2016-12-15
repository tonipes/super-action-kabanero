#include "minebombers/behaviors/PlayerBehavior.hpp"

PlayerBehavior::PlayerBehavior(Node* node) {
  node->addEventReactor([&](GameInputEvent event) {
    auto action = event.action();
    auto isPressed = event.isPressed();

    if (action == UP) {
      moveUp = isPressed;
    } else if (action == RIGHT) {
      moveRight = isPressed;
    } else if (action == DOWN) {
      moveDown = isPressed;
    } else if (action == LEFT) {
      moveLeft = isPressed;
    } else if (action == FIRE_RIGHT) {
      fireRight = isPressed;
    } else if (action == FIRE_DOWN) {
      fireDown = isPressed;
    } else if (action == FIRE_LEFT) {
      fireLeft = isPressed;
    } else if (action == FIRE_UP) {
      fireUp = isPressed;
    } else if (action == FIRE) {
      throwBomb = isPressed;
    }
  });
  node->addEventReactor([&](CollisionEvent event) {
    if(event.collisionMaterialAttachment()->collisionDamage > 0.0f){
      takeDamage = true;
    }
    else if(event.collisionMaterialAttachment()->gunParameters.isDefined()) {
      Services::logger()->debug("Player got new gun");
      _newGun = event.collisionMaterialAttachment()->gunParameters;
    }
  });
}

const float PI = 3.1415927;

auto PlayerBehavior::update(float delta, std::shared_ptr<Node> node) -> void {
  glm::vec3 pos = node->position();

  if (_newGun.isDefined()) {
    node->addAttachment(std::make_shared<GunAttachment>(_newGun.get()));
    _newGun = Option<std::shared_ptr<GunParameters>>();

    Services::messagePublisher()->sendMessage(Message(
      "audioPlayer:clip/reload.ogg",
      std::make_shared<AudioClipEvent>(CLIP_PLAY)
    ));
  }
  auto moveDirection = glm::vec2(0, 0);
  if (moveUp) moveDirection.y += 1;
  if (moveDown) moveDirection.y -= 1;
  if (moveRight) moveDirection.x += 1;
  if (moveLeft) moveDirection.x -= 1;
  if (moveDirection.x != 0 && moveDirection.y != 0) {
    moveDirection = glm::normalize(moveDirection);
  }

  if (!(moveDirection.x == 0 && moveDirection.y == 0)) {
    auto rotationAngle = std::atan2(moveDirection.y, moveDirection.x);
    node->setLocalRotation(glm::angleAxis(rotationAngle, glm::vec3(0, 0, -1)));
  }


  const auto& physAttachment = node->get<PhysicsAttachment>();
  physAttachment.foreach([&](auto phys) {
    phys.setVelocity(
      moveDirection.x * _playerSpeed,
      moveDirection.y * _playerSpeed
    );
  });

  // auto pos = glm::vec2(node->position());

  glm::vec2 fireDirection;
  _fireDelay -= delta;

  if (fireUp) fireDirection.y += 1;
  if (fireDown) fireDirection.y -= 1;
  if (fireRight) fireDirection.x += 1;
  if (fireLeft) fireDirection.x -= 1;
  if (fireDirection.x != 0 && fireDirection.y != 0) {
    fireDirection = glm::normalize(fireDirection);
  }
  auto shoot = fireDirection.x != 0 || fireDirection.y != 0;

  if (shoot && _fireDelay <= 0) {
    auto gunParams = node->get<GunAttachment>().get().parameters();

    _fireDelay = 1.0f / gunParams->fireRate;
    auto random = Services::random();
    for (auto i = 0; i < gunParams->bulletAmount; i++) {
      auto angle = (random->nextFloat() - 0.5f) * PI ;
      auto spreadFactor = gunParams->accuracy;
      auto rotatedDirection = glm::rotate(fireDirection, angle * spreadFactor);
      _bulletsShot++;

      std::shared_ptr<Node> bulletNode;
      std::shared_ptr<b2BodyDef> bodyDef;
      std::shared_ptr<b2FixtureDef> fixtureDef;

      std::tie(bulletNode, bodyDef, fixtureDef) = NodeFactory::createBullet(gunParams);
      bodyDef->position.Set(
        pos.x + fireDirection.x,
        pos.y + fireDirection.y
      );
      bodyDef->linearVelocity.Set(
        rotatedDirection.x * gunParams->bulletSpeed,
        rotatedDirection.y * gunParams->bulletSpeed
      );

      Services::messagePublisher()->sendMessage(Message("gameScene", std::make_shared<CreateNodeEvent>(
        "world/bullets", bulletNode, bodyDef, fixtureDef
      )));
    }

    Services::messagePublisher()->sendMessage(
      Message(
        "audioPlayer:clip/" + gunParams->fireSound,
        std::make_shared<AudioClipEvent>(CLIP_PLAY)
      )
    );
  }

  if (throwBomb) {
    std::shared_ptr<Node> bombNode;
    std::shared_ptr<b2BodyDef> bodyDef;
    std::shared_ptr<b2FixtureDef> fixtureDef;

    std::tie(bombNode, bodyDef, fixtureDef) = NodeFactory::createBomb();
    bodyDef->position.Set(pos.x, pos.y);

    Services::messagePublisher()->sendMessage(Message("gameScene", std::make_shared<CreateNodeEvent>(
      "world/bullets", bombNode, bodyDef, fixtureDef
    )));
    Services::messagePublisher()->sendMessage(Message(
      "audioPlayer:clip/set_bomb.ogg",
      std::make_shared<AudioClipEvent>(CLIP_PLAY)
    ));
  }

  if (takeDamage) {
    Services::messagePublisher()->sendMessage(Message(
      "audioPlayer:clip/pain.ogg",
      std::make_shared<AudioClipEvent>(CLIP_PLAY)
    ));
    takeDamage = false;
  }

  throwBomb = false;
}
