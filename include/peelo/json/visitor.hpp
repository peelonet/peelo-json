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

#include <peelo/json/value.hpp>

namespace peelo::json
{
  class visitor
  {
  public:
    virtual void
    visit_array(const array::container_type& elements) = 0;

    virtual void
    visit_boolean(bool value) = 0;

    virtual void
    visit_null() = 0;

    virtual void
    visit_number(double value) = 0;

    virtual void
    visit_object(const object::container_type& properties) = 0;

    virtual void
    visit_string(const string::value_type& value) = 0;
  };

  inline void
  accept(class visitor& visitor, const value::ptr& value)
  {
    switch (type_of(value))
    {
      case type::array:
        visitor.visit_array(
          std::static_pointer_cast<array>(value)->elements()
        );
        break;

      case type::boolean:
        visitor.visit_boolean(
          std::static_pointer_cast<boolean>(value)->value()
        );
        break;

      case type::null:
        visitor.visit_null();
        break;

      case type::number:
        visitor.visit_number(
          std::static_pointer_cast<number>(value)->value()
        );
        break;

      case type::object:
        visitor.visit_object(
          std::static_pointer_cast<object>(value)->properties()
        );
        break;

      case type::string:
        visitor.visit_string(
          std::static_pointer_cast<string>(value)->value()
        );
        break;
    }
  }
}
