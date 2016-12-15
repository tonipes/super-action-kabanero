#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include "minebombers/level/LevelCompiler.hpp"

KBVector<std::shared_ptr<GunParameters>> LevelCompiler::normalGuns {
  std::make_shared<GunParameters>( 20.0f,  2.0f, 1, 0.1f, 10.0f, 1.0f, "new_tiles/crate",   "test-effect/crystal_spear0", "gunshot.ogg",       false, 1.0f, 0, 0, "Pistol"),
  std::make_shared<GunParameters>(  8.0f,  6.0f, 1, 0.3f, 12.0f, 0.7f, "new_tiles/crate",    "test-effect/crystal_spear0", "gunshot.ogg",       false, 1.0f, 0, 0, "Rifle"),
  std::make_shared<GunParameters>( 10.0f,  1.5f, 3, 0.2f, 10.0f, 0.9f, "new_tiles/crate",  "test-effect/crystal_spear0", "gunshot.ogg",       false, 1.0f, 0, 0, "Shotgun"),
  std::make_shared<GunParameters>( 60.0f,  0.5f, 1, 0.0f, 25.0f, 0.6f, "new_tiles/crate",   "test-effect/crystal_spear0", "gunshot.ogg",       false, 1.0f, 0, 0, "Sniper")
};

KBVector<std::shared_ptr<GunParameters>> LevelCompiler::artifactGuns {
  std::make_shared<GunParameters>(150.0f,  2.0f, 1, 0.1f, 10.0f, 0.5f, "new_tiles/crate",   "test-effect/crystal_spear0", "rocket_launch.ogg", true,  1.0f, 0, 0, "Rocket Launcher"),
  std::make_shared<GunParameters>( 45.0f,  2.0f, 1, 0.05f, 20.0f, 1.1f, "new_tiles/crate",  "test-effect/crystal_spear0", "gunshot.ogg",     false, 1.0f, 0, 0, "Super Pistol"),
  std::make_shared<GunParameters>( 16.0f, 10.0f, 1, 0.25f, 20.0f, 0.7f, "new_tiles/crate",   "test-effect/crystal_spear0", "gunshot.ogg",     false, 1.0f, 0, 0, "Super Rifle"),
  std::make_shared<GunParameters>( 15.0f,  2.5f, 5, 0.15f, 15.0f, 0.9f, "new_tiles/crate", "test-effect/crystal_spear0", "gunshot.ogg",     false, 1.0f, 0, 0, "Super Shotgun"),
  std::make_shared<GunParameters>(150.0f, 0.75f, 1, 0.00f, 45.0f, 0.6f, "new_tiles/crate",  "test-effect/crystal_spear0", "gunshot.ogg",     false, 1.0f, 0, 0, "Super Sniper")
};
