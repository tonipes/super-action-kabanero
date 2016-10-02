#include <iostream>
#include "Dog.hpp"

int main() {
  auto dog = kabanero::Dog("Doge");

  std::cout << "Dog is " << dog.name << std::endl;
  dog.Woof();

  return 0;
}
