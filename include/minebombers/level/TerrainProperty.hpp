#pragma once

#include <string>

/**
 * A data holder class for a given terrain type
 */
class TerrainProperty {
public:
  TerrainProperty() {}
  TerrainProperty(int numVariations, float health, std::string baseName) :
    numVariations(numVariations), health(health), spriteBaseName(baseName) { }

  int numVariations;
  float health;
  std::string spriteBaseName;
};
