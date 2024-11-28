# peelo-json

![Build](https://github.com/peelonet/peelo-json/workflows/Build/badge.svg)

I recently stumbled across an [Hacker News post] that announced an
[alternative] to the widely used [nlohmann JSON] library for C++.

It gave me an idea to test how much work it would be to write a minimal [JSON]
parser library for C++ and this is the result.

I wanted to be as tiny as possible, not to include it's own UTF-8 decoder
(bring your own) and not to throw exceptions but use result type instead.

[Doxygen generated API documentation.][API]

[Hacker News Post]: https://news.ycombinator.com/item?id=42132533
[alternative]: https://github.com/jart/json.cpp
[nlohmann JSON]: https://github.com/nlohmann/json/
[JSON]: https://www.json.org
[API]: https://peelonet.github.io/peelo-json/index.html

## Dependencies

This library depends on another header only library called [peelo-result]. This
should be handled by [CMake].

[peelo-result]: https://github.com/peelonet/peelo-result
[CMake]: https://cmake.org

## Usage

### Parsing JSON

You can use `peelo::json::parse()` function to parse an Unicode string into
JSON value.

```cpp
#include <iostream>
#include <peelo/json.hpp>

int
main()
{
  const auto result = peelo::json::parse(U"[1, 2, 3]");

  if (result)
  {
    if (peelo::json::type_of(*result) == peelo::json::type::array)
    {
      const auto array = std::static_pointer_cast<peelo::json::array>(*result);

      std::cout << "Given input produced an array." << std::endl;
      std::cout << "It has " << array->elements().size() << " elements." << std::endl;
    }
  } else {
    std::cout << "Parsing error occurred: " << result.error().what() << std::endl;
  }
}
```

If you want specicially to parse an JSON object, you can use
`peelo::json::parse_object()` function instead, which does not accept any
other input than an object.

### Formatting JSON

To format an JSON value returned by `peelo::json::parse()` function into an
ASCII string, you can use `peelo::json::format()` function.

```cpp
#include <iostream>
#include <peelo/json.hpp>

int
main()
{
  if (const auto result = peelo::json::parse(U"{\"foo\": \"bar\"}"))
  {
    // This should output `{"foo":"bar"}` to standard output.
    std::cout << peelo::json::format(*value) << std::endl;
  }
}
```

## TODO

- Pretty print option for formatting JSON values.
- `std::u32string` version of `format()` function.
