#pragma once

#include "service/Random.hpp"
#include <cstdlib>

class StdLibRandom : public Random {
public:
  StdLibRandom() {
  }

  auto seed(int seed) -> void {
    std::srand(seed);
  }

  auto nextFloat() -> float {
    return static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX);
  }

  auto nextInt(int mod) -> int {
    return std::rand() % mod;
  }
};
