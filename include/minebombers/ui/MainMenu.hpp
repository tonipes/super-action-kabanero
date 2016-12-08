#pragma once

#include <memory>
#include <iostream>
#include "scene/Node.hpp"
#include "minebombers/ui/MenuBehaviour.hpp"
#include "minebombers/ui/MenuButtonBehaviour.hpp"
#include "minebombers/GameType.hpp"

class MainMenu {
public:
  MainMenu() {}

  auto init() -> std::shared_ptr<Node<Transform3D>> {
    auto main = std::make_shared<Node<Transform3D>>("menu");

    auto spUn = std::make_shared<Node<Transform3D>>("sp_not_selected");
    spUn->addAttachment(std::make_shared<SpriteAttachment>("ui/single_player_unselected"));
    spUn->addBehavior<MenuButtonBehaviour>(0, false, SINGLEPLAYER);
    auto spSel = std::make_shared<Node<Transform3D>>("sp_selected");
    spSel->addAttachment(std::make_shared<SpriteAttachment>("ui/single_player_selected"));
    spSel->addBehavior<MenuButtonBehaviour>(0, true, SINGLEPLAYER);
    auto sp = std::make_shared<Node<Transform3D>>("singleplayer");

    auto coopUn = std::make_shared<Node<Transform3D>>("coop_not_selected");
    coopUn->addAttachment(std::make_shared<SpriteAttachment>("ui/cooperative_unselected"));
    coopUn->addBehavior<MenuButtonBehaviour>(1, false, COOPERATIVE);
    auto coopSel = std::make_shared<Node<Transform3D>>("coop_selected");
    coopSel->addAttachment(std::make_shared<SpriteAttachment>("ui/cooperative_selected"));
    coopSel->addBehavior<MenuButtonBehaviour>(1, true, COOPERATIVE);
    auto coop = std::make_shared<Node<Transform3D>>("coop");

    auto versusUn = std::make_shared<Node<Transform3D>>("versus_not_selected");
    versusUn->addAttachment(std::make_shared<SpriteAttachment>("ui/versus_unselected"));
    versusUn->addBehavior<MenuButtonBehaviour>(2, false, VERSUS);
    auto versusSel = std::make_shared<Node<Transform3D>>("versus_selected");
    versusSel->addAttachment(std::make_shared<SpriteAttachment>("ui/versus_selected"));
    versusSel->addBehavior<MenuButtonBehaviour>(2, true, VERSUS);
    auto versus = std::make_shared<Node<Transform3D>>("versus");

    sp->addChild(spUn);
    sp->addChild(spSel);
    coop->addChild(coopUn);
    coop->addChild(coopSel);
    versus->addChild(versusUn);
    versus->addChild(versusSel);

    sp->setLocalPosition(glm::vec3(150, 150, 0));
    coop->setLocalPosition(glm::vec3(150, 0, 0));
    versus->setLocalPosition(glm::vec3(150, -150, 0));

    main->addChild(sp);
    main->addChild(coop);
    main->addChild(versus);
    main->setIgnoreCamera(true);

    main->addBehavior<MenuBehaviour>();
    return main;
  }
};
