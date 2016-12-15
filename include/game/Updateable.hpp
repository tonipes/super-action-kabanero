#pragma once

/**
 * Updateable interface
 */
class Updateable {
public:
  Updateable() {}
  virtual ~Updateable() {}

  /**
   * Initializes object
   */
  virtual auto init() -> void = 0;

  /**
   * Updates object
   * @param delta time since last update.
   */
  virtual auto update(double delta) -> void = 0;
};
