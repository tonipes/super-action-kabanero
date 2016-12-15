#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

/**
 * An event which tells the hud it should update itself, such as when
 * a players amount of lives changes
 */
class UpdateHudEvent : public Event {
public:
  UpdateHudEvent(std::string text): _text(text) {}

  auto getText() const -> std::string { return _text; }

private:
  std::string _text;

};
