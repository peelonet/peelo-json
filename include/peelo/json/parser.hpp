/*
 * Copyright (c) 2024, Rauli Laine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <cmath>
#include <cctype>
#include <string>

#include <peelo/json/exception.hpp>
#include <peelo/json/value.hpp>
#include <peelo/result.hpp>

namespace peelo::json
{
  using parse_result = result<value::ptr, parse_error>;

  namespace internal
  {
    template<class Iterator>
    parse_result
    parse_value(
      Iterator&,
      const Iterator&,
      position&
    );

    template<class Iterator>
    inline bool
    eof(
      const Iterator& current,
      const Iterator& end
    )
    {
      return current >= end;
    }

    template<class Iterator>
    char32_t
    advance(
      Iterator& current,
      struct position& position
    )
    {
      const auto c = *current++;

      if (c == '\n')
      {
        ++position.line;
        position.column = 1;
      }
      else if (c != '\r')
      {
        ++position.column;
      }

      return c;
    }

    template<class Iterator>
    inline bool
    peek(
      const Iterator& current,
      const Iterator& end,
      char32_t expected
    )
    {
      return !eof(current, end) && *current == expected;
    }

    template<class Iterator>
    inline bool
    peek_digit(
      const Iterator& current,
      const Iterator& end
    )
    {
      return !eof(current, end) && std::isdigit(*current);
    }

    template<class Iterator>
    bool
    peek_advance(
      Iterator& current,
      const Iterator& end,
      struct position& position,
      char32_t expected
    )
    {
      if (peek(current, end, expected))
      {
        advance(current, position);

        return true;
      }

      return false;
    }

    template<class Iterator>
    bool
    eat_whitespace(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      while (!eof(current, end))
      {
        if (!std::isspace(*current))
        {
          return true;
        }
        advance(current, position);
      }

      return false;
    }

    template<class Iterator>
    bool
    eat_digits(
      Iterator& current,
      const Iterator& end,
      struct position& position,
      std::string& buffer
    )
    {
      if (!peek_digit(current, end))
      {
        return false;
      }
      do
      {
        buffer.append(1, static_cast<char>(advance(current, position)));
      }
      while (peek_digit(current, end));

      return true;
    }

    template<class Iterator>
    parse_result
    parse_false(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      if (
        !eat_whitespace(current, end, position) ||
        !peek_advance(current, end, position, 'f') ||
        !peek_advance(current, end, position, 'a') ||
        !peek_advance(current, end, position, 'l') ||
        !peek_advance(current, end, position, 's') ||
        !peek_advance(current, end, position, 'e')
      )
      {
        return parse_result::error({
          position,
          "Unexpected input; Missing `false'."
        });
      }

      return parse_result::ok(std::make_shared<boolean>(false));
    }

    template<class Iterator>
    parse_result
    parse_true(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      if (
        !eat_whitespace(current, end, position) ||
        !peek_advance(current, end, position, 't') ||
        !peek_advance(current, end, position, 'r') ||
        !peek_advance(current, end, position, 'u') ||
        !peek_advance(current, end, position, 'e')
      )
      {
        return parse_result::error({
          position,
          "Unexpected input; Missing `true'."
        });
      }

      return parse_result::ok(std::make_shared<boolean>(true));
    }

    template<class Iterator>
    parse_result
    parse_null(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      if (
        !eat_whitespace(current, end, position) ||
        !peek_advance(current, end, position, 'n') ||
        !peek_advance(current, end, position, 'u') ||
        !peek_advance(current, end, position, 'l') ||
        !peek_advance(current, end, position, 'l')
      )
      {
        return parse_result::error({
          position,
          "Unexpected input; Missing `null'."
        });
      }

      return parse_result::ok(nullptr);
    }

    /**
     * Determines whether given Unicode code point is valid or not.
     */
    inline bool
    is_valid_unicode_codepoint(char32_t c)
    {
      return !(c > 0x10ffff
        || (c & 0xfffe) == 0xfffe
        || (c >= 0xd800 && c <= 0xdfff)
        || (c >= 0xfdd0 && c <= 0xfdef));
    }

    using parse_escape_sequence_result = peelo::result<char32_t, parse_error>;

    template<class Iterator>
    parse_escape_sequence_result
    parse_escape_sequence(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      char32_t result;

      if (eof(current, end))
      {
        return parse_escape_sequence_result::error({
          position,
          "Unexpected end of input; Missing escape sequence."
        });
      }

      if (!peek_advance(current, end, position, U'\\'))
      {
        return parse_escape_sequence_result::error({
          position,
          "Unexpected input; Missing escape sequence."
        });
      }

      if (eof(current, end))
      {
        return parse_escape_sequence_result::error({
          position,
          "Unexpected end of input; Missing escape sequence."
        });
      }

      switch (advance(current, position))
      {
        case 'b':
          result = 010;
          break;

        case 't':
          result = 011;
          break;

        case 'n':
          result = 012;
          break;

        case 'f':
          result = 014;
          break;

        case 'r':
          result = 015;
          break;

        case '"':
        case '\'':
        case '\\':
        case '/':
          result = *(current - 1);
          break;

        case 'u':
          result = 0;
          for (int i = 0; i < 4; ++i)
          {
            if (current >= end)
            {
              return parse_escape_sequence_result::error({
                position,
                "Unterminated escape sequence."
              });
            }
            else if (!std::isxdigit(*current))
            {
              return parse_escape_sequence_result::error({
                position,
                "Illegal Unicode hex escape sequence."
              });
            }

            if (*current >= 'A' && *current <= 'F')
            {
              result = result * 16 + (*current - 'A' + 10);
            }
            else if (*current >= 'a' && *current <= 'f')
            {
              result = result * 16 + (*current - 'a' + 10);
            } else {
              result = result * 16 + (*current - '0');
            }

            advance(current, position);
          }

          if (!is_valid_unicode_codepoint(result))
          {
            return parse_escape_sequence_result::error({
              position,
              "Illegal Unicode hex escape sequence."
            });
          }
          break;

        default:
          return parse_escape_sequence_result::error({
            position,
            "Illegal escape sequence in string literal."
          });
      }

      return parse_escape_sequence_result::ok(result);
    }

    using parse_string_result = peelo::result<std::u32string, parse_error>;

    template<class Iterator>
    parse_string_result
    parse_string(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      struct position start_position;
      std::u32string result;

      if (!eat_whitespace(current, end, position))
      {
        return parse_string_result::error({
          position,
          "Unexpected end of input; Missing string."
        });
      }

      start_position = position;

      if (!peek_advance(current, end, position, U'"'))
      {
        return parse_string_result::error({
          start_position,
          "Unexpected input; Missing string."
        });
      }

      for (;;)
      {
        if (eof(current, end))
        {
          return parse_string_result::error({
            start_position,
            "Unterminated string; Missing `\"'."
          });
        }
        else if (peek_advance(current, end, position, U'"'))
        {
          break;
        }
        else if (peek(current, end, U'\\'))
        {
          const auto escape_sequence = parse_escape_sequence(
            current,
            end,
            position
          );

          if (!escape_sequence)
          {
            return parse_string_result::error(escape_sequence.error());
          }
          result.append(1, *escape_sequence);
        } else {
          result.append(1, advance(current, position));
        }
      }

      return parse_string_result::ok(result);
    }

    template<class Iterator>
    parse_result
    parse_object(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      struct position start_position;
      object::container_type properties;

      if (!eat_whitespace(current, end, position))
      {
        return parse_result::error({
          position,
          "Unexpected end of input; Missing object."
        });
      }

      start_position = position;

      if (!peek_advance(current, end, position, U'{'))
      {
        return parse_result::error({
          start_position,
          "Unexpected input; Missing object."
        });
      }

      // Look for an empty object.
      eat_whitespace(current, end, position);
      if (peek_advance(current, end, position, U'}'))
      {
        return parse_result::ok(std::make_shared<object>());
      }

      for (;;)
      {
        const auto key_result = parse_string(current, end, position);

        if (!key_result)
        {
          return parse_result::error(key_result.error());
        }

        eat_whitespace(current, end, position);
        if (!peek_advance(current, end, position, U':'))
        {
          return parse_result::error({
            start_position,
            "Missing `:' after property key."
          });
        }

        const auto value_result = parse_value(current, end, position);

        if (!value_result)
        {
          return parse_result::error(value_result.error());
        }

        properties[*key_result] = *value_result;

        eat_whitespace(current, end, position);

        if (peek_advance(current, end, position, U','))
        {
          continue;
        }
        else if (!peek_advance(current, end, position, U'}'))
        {
          return parse_result::error({
            start_position,
            "Unterminated object: Missing `}'."
          });
        }

        break;
      }

      return parse_result::ok(std::make_shared<object>(properties));
    }

    template<class Iterator>
    parse_result
    parse_array(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      struct position start_position;
      array::container_type elements;

      if (!eat_whitespace(current, end, position))
      {
        return parse_result::error({
          position,
          "Unexpected end of input; Missing array."
        });
      }

      start_position = position;

      if (!peek_advance(current, end, position, U'['))
      {
        return parse_result::error({
          start_position,
          "Unexpected input; Missing array."
        });
      }

      // Look for an empty array.
      eat_whitespace(current, end, position);
      if (peek_advance(current, end, position, U']'))
      {
        return parse_result::ok(std::make_shared<array>());
      }

      for (;;)
      {
        const auto result = parse_value(current, end, position);

        if (!result)
        {
          return parse_result::error(result.error());
        }

        elements.push_back(*result);

        eat_whitespace(current, end, position);

        if (peek_advance(current, end, position, U','))
        {
          continue;
        }
        else if (!peek_advance(current, end, position, U']'))
        {
          return parse_result::error({
            start_position,
            "Unterminated array: Missing `]'."
          });
        }

        break;
      }

      return parse_result::ok(std::make_shared<array>(elements));
    }

    template<class Iterator>
    parse_result
    parse_number(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      struct position start_position;
      std::string buffer;
      double result;

      if (!eat_whitespace(current, end, position))
      {
        return parse_result::error({
          position,
          "Unexpected end of input; Missing number."
        });
      }

      start_position = position;

      if (peek_advance(current, end, position, U'-'))
      {
        buffer.append(1, '-');
      } else {
        peek_advance(current, end, position, U'+');
      }

      if (!eat_digits(current, end, position, buffer))
      {
        return parse_result::error({
          start_position,
          "Unexpected input; Missing number."
        });
      }

      if (peek_advance(current, end, position, U'.'))
      {
        buffer.append(1, '.');
        if (!eat_digits(current, end, position, buffer))
        {
          return parse_result::error({
            start_position,
            "Unexpected input; Missing digits after `.'."
          });
        }
      }

      if (
        peek_advance(current, end, position, U'e') ||
        peek_advance(current, end, position, U'E')
      )
      {
        buffer.append(1, 'e');
        if (!eat_digits(current, end, position, buffer))
        {
          return parse_result::error({
            start_position,
            "Unexpected input; Missing digits after exponent."
          });
        }
      }

      result = std::strtod(buffer.c_str(), nullptr);
      if (result == HUGE_VAL)
      {
        return parse_result::error({
          start_position,
          "Number out of bounds."
        });
      }

      return parse_result::ok(std::make_shared<number>(result));
    }

    template<class Iterator>
    parse_result
    parse_value(
      Iterator& current,
      const Iterator& end,
      struct position& position
    )
    {
      if (!eat_whitespace(current, end, position))
      {
        return parse_result::error({
          position,
          "Unexpected end of input; Missing value."
        });
      }

      switch (*current)
      {
        case U'[':
          return parse_array(current, end, position);

        case U'{':
          return parse_object(current, end, position);

        case U'"':
          {
            const auto result = parse_string(current, end, position);

            if (result)
            {
              return parse_result::ok(std::make_shared<string>(*result));
            }

            return parse_result::error(result.error());
          }

        case U't':
          return parse_true(current, end, position);

        case U'f':
          return parse_false(current, end, position);

        case U'n':
          return parse_null(current, end, position);

        case U'+':
        case U'-':
        case U'0':
        case U'1':
        case U'2':
        case U'3':
        case U'4':
        case U'5':
        case U'6':
        case U'7':
        case U'8':
        case U'9':
          return parse_number(current, end, position);
      }

      return parse_result::error({
        position,
        "Unexpected input; Missing value."
      });
    }
  }

  inline parse_result
  parse(
    const std::u32string& source,
    int line = 1,
    int column = 1
  )
  {
    auto it = std::begin(source);
    const auto end = std::end(source);
    struct position position{line, column};
    const auto result = internal::parse_value(it, end, position);

    if (!result)
    {
      return result;
    }
    internal::eat_whitespace(it, end, position);
    if (!internal::eof(it, end))
    {
      return parse_result::error({
        position,
        "TODO: Describe me."
      });
    }

    return result;
  }

  inline parse_result
  parse_object(
    const std::u32string& source,
    int line = 1,
    int column = 1
  )
  {
    auto it = std::begin(source);
    const auto end = std::end(source);
    struct position position{line, column};
    const auto result = internal::parse_object(it, end, position);

    if (!result)
    {
      return result;
    }
    internal::eat_whitespace(it, end, position);
    if (!internal::eof(it, end))
    {
      return parse_result::error({
        position,
        "TODO: Describe me."
      });
    }

    return result;
  }
}
