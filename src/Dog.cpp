#include <iostream>

#include "Dog.hpp"

void kabanero::Dog::Woof(){
  std::cout << "Woof! t:" << name << std::endl;
}

auto kabanero::operator<<(std::ostream& os, const Dog& dog) -> std::ostream& {
  os << "Mr. Dog von " << dog.name;
  return os;
}
