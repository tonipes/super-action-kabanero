#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()
#include "catch.hpp"
#include "kabanero/Option.hpp"
#include "kabanero/collection/mutable/Seq.hpp"
#include "kabanero/collection/mutable/Map.hpp"

TEST_CASE( "Option tests", "[option]" ) {
    REQUIRE( Some(5).get() == 5 );
    REQUIRE( Option<int>().getOrElse(12) == 12 );
    REQUIRE( Option<int>().isDefined() == false );
    REQUIRE( Option<int>().isEmpty() == true );
    REQUIRE( Option<int>(5) );
    REQUIRE( !Option<int>() );
}

TEST_CASE( "Seq tests", "[seq]" ) {
  auto s = VectorSeq<int> {1,2,3,4,5};
  auto s2 = VectorSeq<std::string> {"eka", "toka", "kolmas"};
  auto s_copy = VectorSeq<int>(s);
  auto s_mapped = s2.map([](auto a){ return a + "_mapped";});
  s_copy.remove(3);

  REQUIRE( s[3] == 4 );
  REQUIRE( s_copy[3] == 5);
  REQUIRE( s_mapped[0] == "eka_mapped");
}

TEST_CASE( "Map tests", "[map]" ) {
  auto s = MapDict<std::string, int>();
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
