#include <catch2/catch_test_macros.hpp>
#include <peelo/json/formatter.hpp>

using namespace peelo::json;

TEST_CASE("Array is formatted", "[format]")
{
  REQUIRE(
    !format(array::make({
      std::make_shared<number>(1),
      std::make_shared<number>(2),
      std::make_shared<number>(3),
    })).compare("[1,2,3]")
  );
}

TEST_CASE("Boolean is formatted", "[format]")
{
  REQUIRE(!format(boolean::make(true)).compare("true"));
  REQUIRE(!format(boolean::make(false)).compare("false"));
}

TEST_CASE("Null is formatted", "[format]")
{
  REQUIRE(!format(nullptr).compare("null"));
}

TEST_CASE("Number if formatted", "[format]")
{
  REQUIRE(!format(number::make(5)).compare("5"));
  REQUIRE(!format(number::make(3.14)).compare("3.14"));
  REQUIRE(!format(number::make(-500)).compare("-500"));
}

TEST_CASE("Object is formatted", "[format]")
{
  REQUIRE(
    !format(object::make({
      { U"foo", number::make(5) }
    })).compare("{\"foo\":5}")
  );
}

TEST_CASE("String is formatted", "[format]")
{
  REQUIRE(!format(string::make(U"foo")).compare("\"foo\""));
  REQUIRE(!format(string::make(U"\010")).compare("\"\\b\""));
  REQUIRE(!format(string::make(U"\011")).compare("\"\\t\""));
  REQUIRE(!format(string::make(U"\012")).compare("\"\\n\""));
  REQUIRE(!format(string::make(U"\014")).compare("\"\\f\""));
  REQUIRE(!format(string::make(U"\015")).compare("\"\\r\""));
  REQUIRE(!format(string::make(U"\"")).compare("\"\\\"\""));
  REQUIRE(!format(string::make(U"\\")).compare("\"\\\\\""));
  REQUIRE(!format(string::make(U"/")).compare("\"\\/\""));
  REQUIRE(!format(string::make(U"\u00e4")).compare("\"\\u00e4\""));
}
