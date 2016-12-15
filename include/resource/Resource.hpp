#pragma once

#include <string>
#include <iostream>

/**
 * Resource interface.
 */
class Resource {
public:
  Resource() {}
  virtual ~Resource() { std::cout << "~Resource" << std::endl; }

private:
  Resource(const Resource& resource) {}
};
