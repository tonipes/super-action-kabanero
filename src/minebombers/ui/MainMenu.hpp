#pragma once

#include <memory>
#include "scene/Node.hpp"

class MainMenu {
public:
  MainMenu(): selection(0) {}

  auto init() -> std::shared_ptr<Node<Transform3D>> {
    auto main = std::make_shared<Node<Transform3D>>("menu");

    auto spUn = std::make_shared<Node<Transform3D>>("sp_not_selected");
    spUn->addAttachment(std::make_shared<SpriteAttachment>("ui/singleplayer_unselected"));
    auto spSel = std::make_shared<Node<Transform3D>>("sp_selected");
    spSel->addAttachment(std::make_shared<SpriteAttachment>("ui/singleplayer_selected"));
    auto sp = std::make_shared<Node<Transform3D>>("singleplayer");

    sp->addChild(spUn);
    sp->addChild(spSel);


    main->addChild(sp);

    return main;
  }

  auto changeSelection(int dir) {
    selection += dir;
    if (selection > 2) {
      selection = 0;
    } else if (selection < 0) {
      selection = 2;
    }
  }
private:
  int selection;
};
