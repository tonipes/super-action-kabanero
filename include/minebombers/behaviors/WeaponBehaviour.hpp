#pragma once

#include <sstream>
#include "minebombers/events/PlayerLocationEvent.hpp"
#include "message/event/GameInputEvent.hpp"
#include "game/Behavior.hpp"
#include "scene/Node.hpp"
#include "scene/3D/Transform3D.hpp"
#include "service/Services.hpp"
#include <SFML/System/Clock.hpp>
#include <glm/vec2.hpp>
#include "scene/attachment/SpriteAttachment.hpp"




class WeaponBehaviour : public Behavior<Transform3D> {
public:
  //firerate
  WeaponBehaviour(Node<Transform3D>* node, int rpm, int damage, std::string ammoName, std::string name, int magSize) :
  _damage(damage), _node(node), _name(name), _ammoName(ammoName), _magSize(magSize) {
    node->addEventReactor([&](GameInputEvent event) {
      auto action = event.action();
      auto isPressed = event.isPressed();
      if (action == FIRE) {
        _primaryFire = isPressed;
      } else if (action == FIRE1) {
        _secondaryFire = isPressed;
      }
    });
    meterializeAmmo();
    std::ostringstream oss;
    _shootDelay = 1000/(rpm/60.f);
    oss << _shootDelay;
    Services::logger()->debug(oss.str());

  }

  auto update(float delta, Node<Transform3D>& node) -> void override {

    int elapsedMs = _Clock.getElapsedTime().asMilliseconds() - 10;
    std::ostringstream oss;
    if(elapsedMs >= _shootDelay) {
      if(_primaryFire) {
        //shoot1
        _node->addChild(meterializeAmmo());
        oss << elapsedMs;
        Services::logger()->debug(oss.str());
        _Clock.restart();
      }
      if(_secondaryFire) {
        //shoot2
        _node->addChild(meterializeAmmo());
        oss << elapsedMs;
        Services::logger()->debug(oss.str());
        _Clock.restart();
      }
    }


  }


private:
  auto meterializeAmmo()  -> std::shared_ptr<Node<Transform3D>> {
    auto node = std::make_shared<Node<Transform3D>>(_ammoName);
    node->addAttachment(getSprite(_ammoName, -1));
    // node->addBehavior<AmmoBehaviour>(physCircle);
    return node;
  }

  auto getSprite(std::string baseName, int variations) -> std::shared_ptr<SpriteAttachment> {
        return std::make_shared<SpriteAttachment>(baseName);
  }

  int _magSize;
  int ammoinMag = _magSize;
  sf::Clock _Clock;
  Node<Transform3D>* _node;
  std::string _name;
  std::string _ammoName;
  bool _secondaryFire;
  bool _primaryFire;
  int _shootDelay;
  int _damage;

};
