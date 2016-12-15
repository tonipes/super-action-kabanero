#pragma once

#include "message/Event.hpp"
#include <glm/vec2.hpp>

class UpdateHudEvent : public Event {
public:
  UpdateHudEvent(std::string text): _text(text) {}

  auto getText() const -> std::string { return _text; }

private:
  std::string _text;

};
