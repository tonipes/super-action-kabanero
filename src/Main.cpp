#include <iostream>
#include "Dog.hpp"
#include "kabanero/collection/mutable/Seq.hpp"
#include "kabanero/collection/mutable/Map.hpp"
#include "kabanero/Option.hpp"

int main() {
  auto dog = kabanero::Dog("Doge");

  std::cout << "Dog is " << dog.name << std::endl;
  dog.Woof();

  VectorSeq<int> testSeq;
  testSeq += 3;
  testSeq += 10;
  testSeq += 18;
  testSeq += 71;
  testSeq += 41;

  std::cout << testSeq << std::endl;
  testSeq.foreach( [](int elem){ std::cout << elem << std::endl; } );

  std::function<std::string(int)> f = [](auto elem) -> std::string {
    return "bbb";
  };
  auto mapped = testSeq.map([](auto elem) {
    return "aaa";
  });
  auto mapped2 = testSeq.map(f);
  std::cout << mapped << std::endl;
  std::cout << mapped2 << std::endl;
  std::cout << testSeq.fold(0, [](int a, int b) {
    return a + b;
  }) << std::endl;

  auto someString = "Laaadidaa";
  auto test = Some(12);
  auto test2 = Some(someString);
  auto test3 = Some("Tonin stringit");
  auto test4 = Some(0.1234f);
  // std::vector<int> vec = {1, 2, 4};

  auto someOption = Some(mapped2);
  // someOption.get()[1] = "UUUUUU";

  std::cout << someOption << std::endl;
  std::cout << someOption.get() << std::endl;
  std::cout << test << std::endl;
  std::cout << test2 << std::endl;
  std::cout << test3 << std::endl;
  std::cout << test4 << std::endl;

  const auto foundValue = testSeq.find([](int v) {
    return v == 18;
  });

  const auto notFoundValue = testSeq.find([](int v) {
    return v == 99;
  });

  std::cout << "Found value: " << foundValue << std::endl;
  std::cout << "Found value: " << notFoundValue << std::endl;

  std::cout << testSeq.toVector()[2] << std::endl;

  std::cout << testSeq.filter([](auto v) {
    return v % 2 == 0;
  }) << std::endl;

  auto testSeqCopy = VectorSeq<int>(testSeq);
  testSeq += 100;

  std::cout << testSeq << std::endl;
  std::cout << testSeqCopy << std::endl;

  testSeq[0] = 10000;
  std::cout << testSeq << std::endl;
  auto removedElement = testSeq.remove(2);
  std::cout << "Remove index at 2: " << removedElement << std::endl;
  std::cout << testSeq << std::endl;
  // auto someOption2 = Some(3);

  using namespace kabanero;
  VectorSeq<Dog> dogs;
  dogs += Dog("Teuvo");
  dogs += Dog("Henri");
  dogs += Dog("Emma");
  dogs += Dog("Döge");
  dogs += Dog("Musti");

  VectorSeq<Dog> initdogs = { Dog("Taneli"), Dog("Liisa"), Dog("Eino") };
  std::cout << "Initialize dogs: " << initdogs << std::endl;

  std::cout << dogs << std::endl;
  dogs.foreach([](auto& d){ d.name += " II"; });
  std::cout << dogs.find([](auto& d){ return d.name == "asd II";}) << std::endl;
  std::cout << dogs << std::endl;
  std::cout << dogs.remove(2) << std::endl;
  std::cout << dogs << std::endl;

  // Option as a boolean
  auto found = dogs.find([](auto& d){ return d.name == "Teuvon veli";});
  std::cout << "Found dog: " << found << std::endl;
  if(found){
    std::cout << "Unwrapped dog: " << found.get() << std::endl;
  } else {
    std::cout << "Did not find doge :(" << std::endl;
  }

  // Dictionary example
  MapDict<std::string, std::string> stringmap;
  stringmap["first"] = "teststring";
  stringmap["second"] = "anotherstring";
  stringmap["third"] = "randomstring";
  std::cout << stringmap["second"] << std::endl;

  std::cout << "Contains 'first' " << stringmap.contains("first") << std::endl;
  std::cout << "Contains 'asd' " << stringmap.contains("asd") << std::endl;

  std::cout << "Get first " << stringmap.get("first") << std::endl;
  std::cout << "Get asd " << stringmap.get("asd") << std::endl;

  std::cout << "GetOrElse " << stringmap.getOrElse("first", "notfound") << std::endl;
  std::cout << "GetOrElse " << stringmap.getOrElse("asdasd", "notfound") << std::endl;

  std::cout << "keys " << stringmap.keys() << std::endl;
  std::cout << "values " << stringmap.values() << std::endl;

  std::cout << stringmap << std::endl;

  return 0;
}
