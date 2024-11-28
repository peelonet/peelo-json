#include <cassert>

#include <peelo/json/visitor.hpp>

class my_visitor : public peelo::json::visitor
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

  void visit_array(const peelo::json::array::container_type&)
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

  void visit_object(const peelo::json::object::container_type&)
  {
    ++object_count;
  }

  void visit_string(const peelo::json::string::value_type&)
  {
    ++string_count;
  }
};

static void
test_visit_array()
{
  my_visitor visitor;

  peelo::json::accept(visitor, std::make_shared<peelo::json::array>());

  assert(visitor.array_count == 1);
}

static void
test_visit_boolean()
{
  my_visitor visitor;

  peelo::json::accept(visitor, std::make_shared<peelo::json::boolean>(true));
  peelo::json::accept(visitor, std::make_shared<peelo::json::boolean>(false));

  assert(visitor.boolean_count == 2);
}

static void
test_visit_null()
{
  my_visitor visitor;

  peelo::json::accept(visitor, nullptr);

  assert(visitor.null_count == 1);
}

static void
test_visit_number()
{
  my_visitor visitor;

  peelo::json::accept(visitor, std::make_shared<peelo::json::number>(5.2));

  assert(visitor.number_count == 1);
}

static void
test_visit_object()
{
  my_visitor visitor;

  peelo::json::accept(visitor, std::make_shared<peelo::json::object>());

  assert(visitor.object_count == 1);
}

static void
test_visit_string()
{
  my_visitor visitor;

  peelo::json::accept(visitor, std::make_shared<peelo::json::string>());

  assert(visitor.string_count == 1);
}

int
main()
{
  test_visit_array();
  test_visit_boolean();
  test_visit_null();
  test_visit_number();
  test_visit_object();
  test_visit_string();
}
