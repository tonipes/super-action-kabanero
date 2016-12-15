#pragma once

/**
 * A random service, which can be seeded, and the used to request random floats
 * and integers.
 */
class Random {
public:
  Random() {}
  virtual ~Random() {};

  virtual auto seed(int seed) -> void = 0;
  virtual auto nextFloat() -> float = 0;
  virtual auto nextInt(int mod) -> int = 0;
};
/**
 * The default random implementation, if nothing else is provided to Services.
 * Returns numbers which the developer has randomly chosen for this purpose.
 */
class NullRandom : public Random {
public:
  NullRandom() {}

  auto seed(int seed) -> void { }
  auto nextFloat() -> float { return 0.35f; }
  auto nextInt(int mod) -> int { return 4 % mod; }
};
