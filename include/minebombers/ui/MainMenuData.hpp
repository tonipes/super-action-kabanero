#pragma once

#include "collection/mutable/KBVector.hpp"

class MainMenuData {
public:
  MainMenuData() : selection(0) {

  }
  int selection;
  KBVector<std::shared_ptr<Node<Transform3D>>> buttons;

  auto refresh() -> void {
    for (auto i = 0; i < buttons.length() / 2; i++) {
      bool rend = true;
      if (selection == i) {
        rend = !rend;
      }
      buttons[i]->setRenderOn(rend);
      buttons[i+1]->setRenderOn(!rend);
      std::cout << rend << " is on for " << i << "\n";
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
