#pragma once

#include "message/Event.hpp"

class DestroyNodeEvent: public Event {
public:
  DestroyNodeEvent(std::string path):
    _path(path) {}

  auto path() const -> std::string {
    return _path;
  }

private:
  std::string _path;
};
