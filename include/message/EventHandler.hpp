#pragma once

#include "message/Event.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "util/LambdaUtil.hpp"

#include <functional>
#include <memory>

class EventHandler {
public:
  auto handleEvent(std::shared_ptr<Event> event) -> void {
    if (responses.contains(typeid(*event))) {
      responses[typeid(*event)](event);
    }
  }

  template <typename F>
  auto addEventResponse(F&& func) -> void {
    typedef function_traits<F> traits;
    typedef typename traits::template arg<0>::type argType;

    responses[typeid(argType)] = [func{std::move(func)}](const std::shared_ptr<Event>& event) -> void {
      auto castEvent = std::dynamic_pointer_cast<argType>(event);
      func(*castEvent);
    };
  }

private:
  KBTypeMap<std::function<void(const std::shared_ptr<Event>&)>> responses;
};
