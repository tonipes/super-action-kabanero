#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()

#define GLM_FORCE_SWIZZLE

#include "catch.hpp"
#include "util/PrintUtil.hpp"
#include "collection/Option.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "collection/mutable/KBTypeMap.hpp"
#include "scene/2D/Transform2D.hpp"
#include "scene/3D/Transform3D.hpp"
#include "scene/3D/Node3D.hpp"
#include "resource/loader/TextLoader.hpp"
#include "service/ResourceManager.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <tuple>
#include <regex>

TEST_CASE( "Option tests", "[option]" ) {
  REQUIRE( Some(5).get() == 5 );
  REQUIRE( Option<int>().getOrElse(12) == 12 );
  REQUIRE( Option<int>().isDefined() == false );
  REQUIRE( Option<int>().isEmpty() == true );
  REQUIRE( Option<int>(5) );
  REQUIRE( !Option<int>() );
  REQUIRE( Some("ABCD").get() == "ABCD" );
  auto mappedValue = Option<int>(3).map([](int i) {
    return i * 10;
  }).get();
  auto mappedValue2 = Option<int>(3).map([](auto i) {
    return std::to_string(i * 10);
  }).get();
  REQUIRE( mappedValue == 30 );
  REQUIRE( mappedValue2 == "30" );
}

TEST_CASE( "Seq tests", "[seq]" ) {
  auto s = KBVector<int> {1,2,3,4,5};
  auto s2 = KBVector<std::string> {"eka", "toka", "kolmas"};
  auto s_copy = KBVector<int>(s);
  auto s_mapped = s2.map([](auto a){ return a + "_mapped";});
  s_copy.remove(3);

  auto a = KBVector<int> {1, 2, 3, 4, 5};
  auto b = KBVector<std::string> {"one", "two", "three", "four", "five"};
  auto r = Seq<std::vector, std::tuple<int, std::string>> {
    std::make_tuple<int, std::string>(1, "one"),
    std::make_tuple<int, std::string>(2, "two"),
    std::make_tuple<int, std::string>(3, "three"),
    std::make_tuple<int, std::string>(4, "four"),
    std::make_tuple<int, std::string>(5, "five")
  };

  REQUIRE( a.zip(b) == r );

  REQUIRE( s[3] == 4 );
  REQUIRE( s_copy[3] == 5);
  REQUIRE( s_mapped[0] == "eka_mapped");

  auto tupleTest = KBVector<std::tuple<int, std::string>>();
  tupleTest += { 0, "zero" };

  REQUIRE( std::get<0>(tupleTest[0]) == 0);
  REQUIRE( std::get<1>(tupleTest[0]) == "zero");
}

TEST_CASE( "Map tests", "[map]" ) {
  auto s = KBMap<std::string, int>();
  s["eka"] = 1;
  s["toka"] = 2;
  s["kolmas"] = 3;
  s["neljas"] = 4;

  auto s_mapped = s.map([](auto k){
    return std::make_pair(k.first + "_newkey", k.second + 100);
  });
  REQUIRE( s.get("eka") );
  REQUIRE( !s.get("someting") );
  REQUIRE( s.getOrElse("eka", 0) == 1 );
  REQUIRE( s_mapped.get("eka_newkey").get() == 101 );
}

TEST_CASE( "Type Map tests", "[type map]") {
  auto m = KBTypeMap<std::string>();
  m[typeid(int)] = "Int";
  m[typeid(double)] = "Double";
  m[typeid(float)] = "Float";

  int a = 0;
  double d = 0.0;
  float f = 0.0f;

  REQUIRE(m[typeid(a)] == "Int");
  REQUIRE(m[typeid(d)] == "Double");
  REQUIRE(m[typeid(f)] == "Float");

  struct A {};

  struct B : public A {};
  struct C : public A {};

  auto m2 = KBTypeMap<std::shared_ptr<A>>();

  auto b = std::make_shared<B>();
  auto c = std::make_shared<C>();

  m2[typeid(B)] = b;
  m2[typeid(C)] = c;

  REQUIRE(m2[typeid(B)] == b);
  REQUIRE(m2[typeid(C)] == c);
}

TEST_CASE( "Transform tests", "[transform]" ) {
  auto t3d = Transform3D();
  auto t3d2 = Transform3D();

  REQUIRE(t3d.matrix() == t3d2.matrix());

  t3d.setPosition(glm::vec3(2, 0, 1));
  auto m = glm::mat4x4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    2, 0, 1, 1);

  REQUIRE(t3d.matrix() == m);

  t3d.setRotation(glm::quat(0, 1, 0, 0.2));
  m = m * glm::toMat4(glm::quat(0, 1, 0, 0.2));

  REQUIRE(t3d.matrix() == m);

  auto t2d = Transform2D();
  auto m33 = glm::mat3x3();

  REQUIRE(t2d.matrix() == m33);

  t2d.setScale(glm::vec2(2, 1));
  m33 = glm::mat3x3(
    2, 0, 0,
    0, 1, 0,
    0, 0, 1
  );
  REQUIRE(t2d.matrix() == m33);

}

TEST_CASE( "Node tests", "[node]" ) {
  auto node = std::make_shared<Node3D>("node");

  REQUIRE(node->position() == glm::vec3(0, 0, 0));
  REQUIRE(node->rotation() == glm::quat());
  REQUIRE(node->scale() == glm::vec3(1, 1, 1));
  REQUIRE(node->name() == "node");

  auto parent = std::make_shared<Node3D>("parent_node");

  parent->addChild(node);
  REQUIRE(*parent == (node->parent().get()));

  parent->setLocalPosition(glm::vec3(0, 10, 0));

  REQUIRE(node->position() == glm::vec3(0, 10, 0));

  auto child = std::make_shared<Node3D>("child");

  node->addChild(child);

  child->setLocalPosition(glm::vec3(0, 0, 10));

  REQUIRE(child->position() == glm::vec3(0, 10, 10));

  node->setLocalRotation(glm::angleAxis(glm::half_pi<float>(), glm::vec3(0, 1, 0)));

  REQUIRE(glm::round(child->position()) == glm::vec3(10, 10, 0));

  parent->setLocalPosition(glm::vec3(0, 0, 0));

  REQUIRE(node->parent().get().position() == glm::vec3(0, 0, 0));
}

// TEST_CASE( "Resource loading", "[resource]") {
//   auto loader = std::make_shared<TextLoader>();
//   auto text = loader->load("resources/test.txt");
//   REQUIRE(*(std::dynamic_pointer_cast<Text>(text)) == Text("Test string"));
//
//   auto resourceManager = ResourceManager();
//   std::regex text_regex("^.+\\.txt$");
//   resourceManager.addLoader(text_regex, loader);
//
//   resourceManager.load("resources/test.txt");
//
//   auto text2 = resourceManager.get<Text>("resources/test.txt");
//
//   REQUIRE(text2.isDefined());
//   REQUIRE(text2.get().str() == "Test string");
// }
//
// TEST_CASE( "Resource loading exception", "[resource]") {
//   auto loader = std::make_shared<TextLoader>();
//
//   auto resourceManager = ResourceManager();
//   std::regex text_regex("^.+\\.txt$");
//   resourceManager.addLoader(text_regex, loader);
//
//   REQUIRE_THROWS(resourceManager.load("resources/nonexistent.null"));
//
//   auto f = [&](){
//     resourceManager.load("resources/nonexistent.txt");
//   };
//
//   REQUIRE_THROWS(f());
//
// }
