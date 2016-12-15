#pragma once

#include "service/Random.hpp"
#include <cstdlib>

/**
 * A Random-service implementation using the random found in the standard library
 */
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
    if (mod == 0) return 0;
    return std::rand() % mod;
  }
};
