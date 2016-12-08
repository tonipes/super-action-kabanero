#pragma once

#include <memory>
#include <iostream>
#include "scene/Node.hpp"
#include "minebombers/ui/MainMenuData.hpp"
#include "minebombers/ui/MenuBehaviour.hpp"

class MainMenu {
public:
  MainMenu(MainMenuData& data): _data(data) {}

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

    _data.buttons = KBVector<std::shared_ptr<Node<Transform3D>>>();

    _data.buttons += spUn;
    _data.buttons += spSel;
    _data.buttons += coopUn;
    _data.buttons += coopSel;

    refresh();
    main->addBehavior<MenuBehaviour>(_data);

    return main;
  }

private:
  MainMenuData _data;
};
