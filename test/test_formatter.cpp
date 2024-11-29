#include <cassert>

#include <peelo/json/formatter.hpp>

using namespace peelo::json;

static void
test_array()
{
  assert(
    !format(array::make({
      std::make_shared<number>(1),
      std::make_shared<number>(2),
      std::make_shared<number>(3),
    })).compare("[1,2,3]")
  );
}

static void
test_boolean()
{
  assert(!format(boolean::make(true)).compare("true"));
  assert(!format(boolean::make(false)).compare("false"));
}

static void
test_null()
{
  assert(!format(nullptr).compare("null"));
}

static void
test_number()
{
  assert(!format(number::make(5)).compare("5"));
  assert(!format(number::make(3.14)).compare("3.14"));
  assert(!format(number::make(-500)).compare("-500"));
}

static void
test_object()
{
  assert(
    !format(object::make({
      { U"foo", number::make(5) }
    })).compare("{\"foo\":5}")
  );
}

static void
test_string()
{
  assert(!format(string::make(U"foo")).compare("\"foo\""));
  assert(!format(string::make(U"\010")).compare("\"\\b\""));
  assert(!format(string::make(U"\011")).compare("\"\\t\""));
  assert(!format(string::make(U"\012")).compare("\"\\n\""));
  assert(!format(string::make(U"\014")).compare("\"\\f\""));
  assert(!format(string::make(U"\015")).compare("\"\\r\""));
  assert(!format(string::make(U"\"")).compare("\"\\\"\""));
  assert(!format(string::make(U"\\")).compare("\"\\\\\""));
  assert(!format(string::make(U"/")).compare("\"\\/\""));
  assert(!format(string::make(U"\u00e4")).compare("\"\\u00e4\""));
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
