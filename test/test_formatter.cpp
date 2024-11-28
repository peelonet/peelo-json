#include <cassert>

#include <peelo/json/formatter.hpp>

using peelo::json::format;

static void
test_array()
{
  using peelo::json::array;
  using peelo::json::number;
  using peelo::json::value;

  const array::container_type elements =
  {
    std::make_shared<number>(1),
    std::make_shared<number>(2),
    std::make_shared<number>(3),
  };

  assert(!format(std::make_shared<array>(elements)).compare("[1,2,3]"));
}

static void
test_boolean()
{
  using peelo::json::boolean;

  assert(!format(std::make_shared<boolean>(true)).compare("true"));
  assert(!format(std::make_shared<boolean>(false)).compare("false"));
}

static void
test_null()
{
  assert(!format(nullptr).compare("null"));
}

static void
test_number()
{
  using peelo::json::number;

  assert(!format(std::make_shared<number>(5)).compare("5"));
  assert(!format(std::make_shared<number>(3.14)).compare("3.14"));
  assert(!format(std::make_shared<number>(-500)).compare("-500"));
}

static void
test_object()
{
  using peelo::json::number;
  using peelo::json::object;

  const object::container_type properties =
  {
    { U"foo", std::make_shared<number>(5) },
  };

  assert(!format(std::make_shared<object>(properties)).compare("{\"foo\":5}"));
}

static void
test_string()
{
  using peelo::json::string;

  assert(!format(std::make_shared<string>(U"foo")).compare("\"foo\""));
  assert(!format(std::make_shared<string>(U"\010")).compare("\"\\b\""));
  assert(!format(std::make_shared<string>(U"\011")).compare("\"\\t\""));
  assert(!format(std::make_shared<string>(U"\012")).compare("\"\\n\""));
  assert(!format(std::make_shared<string>(U"\014")).compare("\"\\f\""));
  assert(!format(std::make_shared<string>(U"\015")).compare("\"\\r\""));
  assert(!format(std::make_shared<string>(U"\"")).compare("\"\\\"\""));
  assert(!format(std::make_shared<string>(U"\\")).compare("\"\\\\\""));
  assert(!format(std::make_shared<string>(U"/")).compare("\"\\/\""));
  assert(!format(std::make_shared<string>(U"\u00e4")).compare("\"\\u00e4\""));
}

int
main()
{
  test_array();
  test_boolean();
  test_null();
  test_number();
  test_object();
  test_string();
}
