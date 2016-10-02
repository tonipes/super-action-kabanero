#include <string>

namespace kabanero{
  class Dog {

  public:
    Dog(std::string _name): name(_name){};
    std::string name;
    void Woof();

  };
}
