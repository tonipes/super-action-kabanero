#pragma once

#include "resource/Resource.hpp"
#include "collection/mutable/KBMap.hpp"
#include "collection/Option.hpp"
#include "resource/resource/sprite/Sprite.hpp"

class Atlas : public Resource {
public:
  Atlas(KBMap<std::string, Sprite> sprites) : _sprites(sprites) { }

  auto get(std::string id) const -> Option<Sprite> {
    return _sprites.get(id);
  }

private:
  KBMap<std::string, Sprite> _sprites;
};
