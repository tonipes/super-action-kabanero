#pragma once

#include <memory>
#include <iostream>
#include "scene/Node.hpp"

class MenuController {
public:
  MenuController(): selection(0) {}
  KBVector<std::shared_ptr<Node<Transform3D>>> buttons;
  int selection;

  auto refresh() -> void {
    for (auto i = 0; i < buttons.length() / 2; i++) {
      bool rend = true;
      if (selection == i) {
        rend = !rend;
      }
      buttons[i]->setRenderOn(rend);
      buttons[i+1]->setRenderOn(!rend);
    }
  }

  auto changeSelection(int dir) -> void {
    selection += dir;
    if (selection >= buttons.length() / 2) {
      selection = 0;
    } else if (selection < 0) {
      selection = buttons.length() / 2 - 1;
    }
    refresh();
  }

  auto select() {

  }
};
