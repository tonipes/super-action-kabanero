#pragma once

#include "resource/Resource.hpp"
#include "collection/mutable/KBMap.hpp"
#include "resource/resource/sprite/Sprite.hpp"

class Atlas : public Resource {
public:
  Atlas(KBMap<std::string, Sprite> sprites) : _sprites(sprites) { }

private:
  KBMap<std::string, Sprite> _sprites;
};
