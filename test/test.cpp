#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()
#include "catch.hpp"
#include "util/PrintUtil.hpp"
#include "kabanero/Option.hpp"
#include "kabanero/collection/mutable/KBVector.hpp"
#include "kabanero/collection/mutable/KBMap.hpp"
#include "scene/2D/Transform2D.hpp"
#include "scene/3D/Transform3D.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

TEST_CASE( "Option tests", "[option]" ) {
  REQUIRE( Some(5).get() == 5 );
  REQUIRE( Option<int>().getOrElse(12) == 12 );
  REQUIRE( Option<int>().isDefined() == false );
  REQUIRE( Option<int>().isEmpty() == true );
  REQUIRE( Option<int>(5) );
  REQUIRE( !Option<int>() );
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

  REQUIRE( s[3] == 4 );
  REQUIRE( s_copy[3] == 5);
  REQUIRE( s_mapped[0] == "eka_mapped");
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
