#pragma once

#include <string>
#include <iostream>

namespace kabanero{
  class Dog {

  public:
    Dog(std::string _name): name(_name){};
    std::string name;
    void Woof();

  };

  auto operator<<(std::ostream& os, const Dog& dog) -> std::ostream&;
}
