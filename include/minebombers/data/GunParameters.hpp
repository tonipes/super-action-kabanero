#pragma once

#include <string>

class GunParameters {
public:
  GunParameters(
    float dmg,
    float fr,
    int ba,
    float acc,
    float sped,
    std::string sprite,
    std::string bltSprite,
    std::string sound,
    bool rocket = false,
    float expSize = 0,
    int shrapnel = 0,
    int bltSize = 0,
    std::string name = "Gun"):
  damage(dmg),
  fireRate(fr),
  bulletAmount(ba),
  accuracy(acc),
  bulletSpeed(sped),
  sprite(sprite),
  bulletSprite(bltSprite),
  isRocketLauncher(rocket),
  fireSound(sound),
  explosionSize(expSize),
  shrapnelCount(shrapnel),
  bulletSize(bltSize),
  gunName(name)
   {}

  float damage;
  float fireRate; // per second
  int bulletAmount; // bullets per shoot
  float accuracy; //smaller == better
  float bulletSpeed;
  std::string sprite;
  std::string bulletSprite;
  bool isRocketLauncher;
  float explosionSize;
  int shrapnelCount;
  float bulletSize;
  std::string fireSound;
  std::string gunName;
};
