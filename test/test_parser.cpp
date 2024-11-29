#include <catch2/catch_test_macros.hpp>
#include <peelo/json/parser.hpp>

using namespace peelo::json;

TEST_CASE("False boolean value is parsed", "[parse]")
{
  const auto result = parse(U"false");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::boolean);
  REQUIRE(as<boolean>(*result)->value() == false);
}

TEST_CASE("True boolean value is parsed", "[parse]")
{
  const auto result = parse(U"true");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::boolean);
  REQUIRE(as<boolean>(*result)->value() == true);
}

TEST_CASE("Null value is parsed", "[parse]")
{
  const auto result = parse(U"null");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::null);
}

TEST_CASE("String value is parsed", "[parse]")
{
  const auto result = parse(U"\"foo bar\"");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::string);
  REQUIRE(!as<string>(*result)->value().compare(U"foo bar"));
}

TEST_CASE("String value with escape sequences is parsed", "[parse]")
{
  const auto result = parse(U"\"\\b\\t\\n\\f\\r\\\"\\'\\\\\\/\\u00e4\"");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::string);
  REQUIRE(!as<string>(*result)->value().compare(
    U"\010\011\012\014\015\"'\\/\u00e4"
  ));
}

TEST_CASE("Unterminated string value produces error", "[parse]")
{
  const auto result = parse(U"\"foo bar");

  REQUIRE(!result.has_value());
}

TEST_CASE("String with unterminated escape sequence produces error", "[parse]")
{
  const auto result = parse(U"\\u");

  REQUIRE(!result.has_value());
}

TEST_CASE("Integer number is parsed", "[parse]")
{
  const auto result = parse(U"15");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::number);
  REQUIRE(as<number>(*result)->value() == 15);
}

TEST_CASE("Decimal number is parsed", "[parse]")
{
  const auto result = parse(U"3.5");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::number);
  REQUIRE(as<number>(*result)->value() == 3.5);
}

TEST_CASE("Decimal number with exponent is parsed", "[parse]")
{
  const auto result = parse(U"1.2e15");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::number);
  REQUIRE(as<number>(*result)->value() == 1.2e15);
}

TEST_CASE("Negative number is parsed", "[parse]")
{
  const auto result = parse(U"-500");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::number);
  REQUIRE(as<number>(*result)->value() == -500);
}

TEST_CASE("Positive number is parsed", "[parse]")
{
  const auto result = parse(U"+28");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::number);
  REQUIRE(as<number>(*result)->value() == 28);
}

TEST_CASE("Number out of bounds produces error", "[parse]")
{
  const auto result = parse(
    U"123456789"
    U"e"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
    U"123456789"
  );

  REQUIRE(!result.has_value());
}

TEST_CASE("Array is parsed", "[parse]")
{
  const auto result = parse(U"[1, 2, 3]");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::array);
  REQUIRE(as<array>(*result)->elements().size() == 3);
}

TEST_CASE("Empty array is parsed", "[parse]")
{
  const auto result = parse(U"[]");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::array);
  REQUIRE(as<array>(*result)->elements().empty());
}

TEST_CASE("Unterminated array produces error", "[parse]")
{
  const auto result = parse(U"[1, 2");

  REQUIRE(!result.has_value());
}

TEST_CASE("Array with missing comma produces error", "[parse]")
{
  const auto result = parse(U"[1 2");

  REQUIRE(!result.has_value());
}

TEST_CASE("Object is parsed", "[parse]")
{
  const auto result = parse(U"{\"foo\": \"bar\"}");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::object);
  REQUIRE(as<object>(*result)->properties().size() == 1);
}

TEST_CASE("Empty object is parsed", "[parse]")
{
  const auto result = parse(U"{}");

  REQUIRE(result.has_value());
  REQUIRE(type_of(*result) == type::object);
  REQUIRE(as<object>(*result)->properties().empty());
}

TEST_CASE("Unterminated object produces error", "[parse]")
{
  const auto result = parse(U"{\"foo\": \"bar\"");

  REQUIRE(!result.has_value());
}

TEST_CASE("Object with missing comma produces error", "[parse]")
{
  const auto result = parse(U"{\"foo\": \"bar\" \"bar\": \"foo\"}");

  REQUIRE(!result.has_value());
}

TEST_CASE("Value with trailing garbage produces error", "[parse]")
{
  const auto result = parse(U"5 true");

  REQUIRE(!result.has_value());
}

TEST_CASE("Object with trailing garbage produces error", "[parse_object]")
{
  const auto result = parse_object(U"{\"foo\": \"bar\"} null");

  REQUIRE(!result.has_value());
}

TEST_CASE("Parsing object with non-object input produces error", "[parse]")
{
  const auto result = parse_object(U"[1, 2, 3]");

  REQUIRE(!result.has_value());
}
