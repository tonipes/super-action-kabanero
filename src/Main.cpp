#include <iostream>
#include <selene.h>
#include "Dog.hpp"

int main() {
  auto dog = kabanero::Dog("Doge");

  sel::State state;
  state.Load("resources/script/test.lua");

  auto foo = state["foo"];
  auto bar_3 = state["bar"][3];
  auto bar_key = state["bar"]["key"];

  std::cout << int(foo) << std::endl;
  std::cout << std::to_string(bar_3 == "asas") << std::endl;
  std::cout << std::to_string(bar_key == "there") << std::endl;

  std::cout << "Dog is " << dog.name << std::endl;
  dog.Woof();

  return 0;
}
