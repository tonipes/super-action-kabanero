#pragma once

#include <string>

class GunAttachment : public NodeAttachment {
public:
  GunAttachment(float dmg, float fr, int ba, float acc, float sped, std::string spri):
  damage(dmg),
  fireRate(fr),
  bulletAmount(ba),
  accuracy(acc),
  bulletSpeed(sped),
  sprite(spri)
   {}
  // public magic, avoiding boilerplate since 1995
  float damage;
  float fireRate; // per second
  int bulletAmount; // bullets per shoot
  float accuracy; //smaller == better
  float bulletSpeed;
  std::string sprite;
};
