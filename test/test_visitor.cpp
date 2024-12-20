#include <catch2/catch_test_macros.hpp>
#include <peelo/json/visitor.hpp>

using namespace peelo::json;

class my_visitor : public visitor
{
public:
  int array_count;
  int boolean_count;
  int null_count;
  int number_count;
  int object_count;
  int string_count;

  explicit my_visitor()
    : array_count(0)
    , boolean_count(0)
    , null_count(0)
    , number_count(0)
    , object_count(0)
    , string_count(0) {}

  void visit_array(const array::container_type&)
  {
    ++array_count;
  }

  void visit_boolean(bool)
  {
    ++boolean_count;
  }

  void visit_null()
  {
    ++null_count;
  }

  void visit_number(double)
  {
    ++number_count;
  }

  void visit_object(const object::container_type&)
  {
    ++object_count;
  }

  void visit_string(const string::value_type&)
  {
    ++string_count;
  }
};

TEST_CASE("Visitor should detect JSON value types", "[accept]")
{
  my_visitor visitor;

  accept(visitor, array::make({}));
  accept(visitor, boolean::make(true));
  accept(visitor, boolean::make(false));
  accept(visitor, nullptr);
  accept(visitor, number::make(5.2));
  accept(visitor, object::make({}));
  accept(visitor, string::make(U""));

  REQUIRE(visitor.array_count == 1);
  REQUIRE(visitor.boolean_count == 2);
  REQUIRE(visitor.null_count == 1);
  REQUIRE(visitor.number_count == 1);
  REQUIRE(visitor.object_count == 1);
  REQUIRE(visitor.string_count == 1);
}
