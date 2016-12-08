#pragma once

#include <string>

class TerrainProperty {
public:
  TerrainProperty() {}
  TerrainProperty(int numVariations, float health, std::string baseName) :
    numVariations(numVariations), health(health), spriteBaseName(baseName) { }

  int numVariations;
  float health;
  std::string spriteBaseName;
};
