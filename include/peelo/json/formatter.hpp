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

#include <peelo/json/visitor.hpp>

namespace peelo::json
{
  namespace internal
  {
    class formatter final : public visitor
    {
    public:
      formatter() = default;
      formatter(const formatter&) = default;
      formatter(formatter&&) = default;
      formatter& operator=(const formatter&) = default;
      formatter& operator=(formatter&&) = default;

      inline const std::string& result() const
      {
        return m_result;
      }

      void visit_array(const array::container_type& elements)
      {
        bool first = true;

        m_result.append(1, '[');
        for (const auto& element : elements)
        {
          if (first)
          {
            first = false;
          } else {
            m_result.append(1, ',');
          }
          accept(*this, element);
        }
        m_result.append(1, ']');
      }

      void visit_boolean(bool value)
      {
        m_result.append(value ? "true" : "false");
      }

      void visit_null()
      {
        m_result.append("null");
      }

      void visit_number(double value)
      {
        char buffer[32];

        std::snprintf(buffer, sizeof(buffer), "%g", value);
        m_result.append(buffer);
      }

      void visit_object(const object::container_type& properties)
      {
        bool first = true;

        m_result.append(1, '{');
        for (const auto& property : properties)
        {
          if (first)
          {
            first = false;
          } else {
            m_result.append(1, ',');
          }
          output_string(property.first);
          m_result.append(1, ':');
          accept(*this, property.second);
        }
        m_result.append(1, '}');
      }

      void visit_string(const string::value_type& value)
      {
        output_string(value);
      }

    private:
      void output_string(const string::value_type& value)
      {
        m_result.append(1, '"');
        for (const auto& c : value)
        {
          switch (c)
          {
            case 010:
              m_result.append(1, '\\');
              m_result.append(1, 'b');
              break;

            case 011:
              m_result.append(1, '\\');
              m_result.append(1, 't');
              break;

            case 012:
              m_result.append(1, '\\');
              m_result.append(1, 'n');
              break;

            case 014:
              m_result.append(1, '\\');
              m_result.append(1, 'f');
              break;

            case 015:
              m_result.append(1, '\\');
              m_result.append(1, 'r');
              break;

            case '"':
            case '\\':
            case '/':
              m_result.append(1, '\\');
              m_result.append(1, c);
              break;

            default:
              if (!std::isprint(c))
              {
                char buffer[7];

                std::snprintf(buffer, 7, "\\u%04x", c);
                m_result.append(buffer);
              } else {
                m_result.append(1, static_cast<char>(c));
              }
          }
        }
        m_result.append(1, '"');
      }

    private:
      std::string m_result;
    };
  }

  /**
   * Converts given JSON value into ASCII string.
   */
  inline std::string
  format(const value::ptr& value)
  {
    internal::formatter fmt;

    accept(fmt, value);

    return fmt.result();
  }
}
