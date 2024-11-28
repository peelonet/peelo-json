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

#include <exception>
#include <string>

namespace peelo::json
{
  /**
   * Represents position in source code.
   */
  struct position
  {
    int line;
    int column;
  };

  /**
   * Exception type used when parsing JSON fails for some reason.
   */
  class parse_error : public std::exception
  {
  public:
    parse_error(const struct position& position, const std::string& message)
      : m_position(position)
      , m_message(message) {}

    parse_error(const parse_error&) = default;
    parse_error(parse_error&&) = default;
    parse_error& operator=(const parse_error&) = default;
    parse_error& operator=(parse_error&&) = default;

    inline const struct position& position() const
    {
      return m_position;
    }

    inline const char* what() const noexcept
    {
      return m_message.c_str();
    }

  private:
    struct position m_position;
    std::string m_message;
  };
}
