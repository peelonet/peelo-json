#include <cassert>

#include <peelo/json/parser.hpp>

using peelo::json::parse;
using peelo::json::parse_object;
using peelo::json::type;
using peelo::json::type_of;

static void
test_parse_false()
{
  using peelo::json::boolean;

  const auto result = parse(U"false");

  assert(result.has_value());
  assert(type_of(*result) == type::boolean);
  assert(std::static_pointer_cast<boolean>(*result)->value() == false);
}

static void
test_parse_true()
{
  using peelo::json::boolean;

  const auto result = parse(U"true");

  assert(result.has_value());
  assert(type_of(*result) == type::boolean);
  assert(std::static_pointer_cast<boolean>(*result)->value() == true);
}

static void
test_parse_null()
{
  const auto result = parse(U"null");

  assert(type_of(*result) == type::null);
}

static void
test_parse_string()
{
  using peelo::json::string;

  const auto result = parse(U"\"foo bar\"");

  assert(result.has_value());
  assert(type_of(*result) == type::string);
  assert(
    !std::static_pointer_cast<string>(*result)->value().compare(U"foo bar")
  );
}

static void
test_parse_string_with_escape_sequences()
{
  using peelo::json::string;

  const auto result = parse(U"\"\\b\\t\\n\\f\\r\\\"\\'\\\\\\/\\u00e4\"");

  assert(result.has_value());
  assert(type_of(*result) == type::string);
  assert(
    !std::static_pointer_cast<string>(*result)->value().compare(
      U"\010\011\012\014\015\"'\\/\u00e4"
    )
  );
}

static void
test_parse_unterminated_string()
{
  const auto result = parse(U"\"foo bar");

  assert(!result.has_value());
}

static void
test_parse_string_with_unterminated_escape_sequence()
{
  const auto result = parse(U"\\u");

  assert(!result.has_value());
}

static void
test_parse_integer()
{
  using peelo::json::number;

  const auto result = parse(U"15");

  assert(result.has_value());
  assert(type_of(*result) == type::number);
  assert(std::static_pointer_cast<number>(*result)->value() == 15);
}

static void
test_parse_decimal()
{
  using peelo::json::number;

  const auto result = parse(U"3.5");

  assert(result.has_value());
  assert(type_of(*result) == type::number);
  assert(std::static_pointer_cast<number>(*result)->value() == 3.5);
}

static void
test_parse_decimal_with_exponent()
{
  using peelo::json::number;

  const auto result = parse(U"1.2e15");

  assert(result.has_value());
  assert(type_of(*result) == type::number);
  assert(std::static_pointer_cast<number>(*result)->value() == 1.2e15);
}

static void
test_parse_negative_number()
{
  using peelo::json::number;

  const auto result = parse(U"-500");

  assert(result.has_value());
  assert(type_of(*result) == type::number);
  assert(std::static_pointer_cast<number>(*result)->value() == -500);
}

static void
test_parse_positive_number()
{
  using peelo::json::number;

  const auto result = parse(U"+28");

  assert(result.has_value());
  assert(type_of(*result) == type::number);
  assert(std::static_pointer_cast<number>(*result)->value() == 28);
}

static void
test_parse_out_of_bounds_number()
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

  assert(!result.has_value());
}

static void
test_parse_array()
{
  using peelo::json::array;

  const auto result = parse(U"[1, 2, 3]");

  assert(result.has_value());
  assert(type_of(*result) == type::array);
  assert(std::static_pointer_cast<array>(*result)->elements().size() == 3);
}

static void
test_parse_empty_array()
{
  using peelo::json::array;

  const auto result = parse(U"[]");

  assert(result.has_value());
  assert(type_of(*result) == type::array);
  assert(std::static_pointer_cast<array>(*result)->elements().empty());
}

static void
test_parse_unterminated_array()
{
  const auto result = parse(U"[1, 2");

  assert(!result.has_value());
}

static void
test_parse_array_with_missing_comma()
{
  const auto result = parse(U"[1 2");

  assert(!result.has_value());
}

static void
test_parse_object()
{
  using peelo::json::object;

  const auto result = parse(U"{\"foo\": \"bar\"}");

  assert(result.has_value());
  assert(type_of(*result) == type::object);
  assert(std::static_pointer_cast<object>(*result)->properties().size() == 1);
}

static void
test_parse_empty_object()
{
  using peelo::json::object;

  const auto result = parse(U"{}");

  assert(result.has_value());
  assert(type_of(*result) == type::object);
  assert(std::static_pointer_cast<object>(*result)->properties().empty());
}

static void
test_parse_unterminated_object()
{
  const auto result = parse(U"{\"foo\": \"bar\"");

  assert(!result.has_value());
}

static void
test_parse_object_with_missing_comma()
{
  const auto result = parse(U"{\"foo\": \"bar\" \"bar\": \"foo\"}");

  assert(!result.has_value());
}

static void
test_parse_value_with_junk()
{
  const auto result = parse(U"5 true");

  assert(!result.has_value());
}

static void
test_parse_object_with_junk()
{
  const auto result = parse_object(U"{\"foo\": \"bar\"} null");

  assert(!result.has_value());
}

static void
test_parse_object_with_non_object_input()
{
  const auto result = parse_object(U"[1, 2, 3]");

  assert(!result.has_value());
}

int
main()
{
  test_parse_false();
  test_parse_true();
  test_parse_null();
  test_parse_string();
  test_parse_string_with_escape_sequences();
  test_parse_unterminated_string();
  test_parse_string_with_unterminated_escape_sequence();
  test_parse_integer();
  test_parse_decimal();
  test_parse_decimal_with_exponent();
  test_parse_negative_number();
  test_parse_positive_number();
  test_parse_out_of_bounds_number();
  test_parse_array();
  test_parse_empty_array();
  test_parse_unterminated_array();
  test_parse_array_with_missing_comma();
  test_parse_object();
  test_parse_empty_object();
  test_parse_unterminated_object();
  test_parse_object_with_missing_comma();
  test_parse_value_with_junk();
  test_parse_object_with_junk();
  test_parse_object_with_non_object_input();
}
