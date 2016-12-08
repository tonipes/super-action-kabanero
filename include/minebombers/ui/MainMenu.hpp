#pragma once

#include <memory>
#include <iostream>
#include "scene/Node.hpp"
// #include "minebombers/ui/MenuBehaviour.hpp"
class MenuBehaviour;

class MainMenu {
public:
  MainMenu(): selection(0) {}

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

  auto init() -> std::shared_ptr<Node<Transform3D>> {
    auto main = std::make_shared<Node<Transform3D>>("menu");

    auto spUn = std::make_shared<Node<Transform3D>>("sp_not_selected");
    spUn->addAttachment(std::make_shared<SpriteAttachment>("ui/single_player_unselected"));
    auto spSel = std::make_shared<Node<Transform3D>>("sp_selected");
    spSel->addAttachment(std::make_shared<SpriteAttachment>("ui/single_player_selected"));
    auto sp = std::make_shared<Node<Transform3D>>("singleplayer");

    auto coopUn = std::make_shared<Node<Transform3D>>("coop_not_selected");
    coopUn->addAttachment(std::make_shared<SpriteAttachment>("ui/cooperative_unselected"));
    auto coopSel = std::make_shared<Node<Transform3D>>("coop_selected");
    coopSel->addAttachment(std::make_shared<SpriteAttachment>("ui/cooperative_selected"));
    auto coop = std::make_shared<Node<Transform3D>>("coop");

    sp->addChild(spUn);
    sp->addChild(spSel);
    coop->addChild(coopUn);
    coop->addChild(coopSel);


    sp->setLocalPosition(glm::vec3(15, 15, 0));
    coop->setLocalPosition(glm::vec3(15, 115, 0));

    main->addChild(sp);
    main->addChild(coop);
    main->setIgnoreCamera(true);
    //main->addBehavior(std::make_shared<MenuBehaviour>(*this));

    buttons = KBVector<std::shared_ptr<Node<Transform3D>>>();

    buttons += spUn;
    buttons += spSel;
    buttons += coopUn;
    buttons += coopSel;

    refresh();

    return main;
  }


  auto changeSelection(int dir) {
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
private:
  int selection;
  KBVector<std::shared_ptr<Node<Transform3D>>> buttons;
};
