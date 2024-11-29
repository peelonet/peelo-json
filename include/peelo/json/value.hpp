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

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace peelo::json
{
  /**
   * Enumeration of different JSON value types.
   */
  enum class type
  {
    array = 0,
    boolean = 1,
    null = 2,
    number = 3,
    object = 4,
    string = 5,
  };

  namespace internal
  {
    /**
     * Abstract base class for all JSON values.
     */
    class base
    {
    public:
      base() = default;
      base(const base&) = delete;
      base(base&&) = delete;
      void operator=(const base&) = delete;
      void operator=(base&&) = delete;

      /**
       * Returns type of the value.
       */
      virtual enum type type() const = 0;
    };
  }

  /**
   * Representation of JSON value. If the pointer is `null`, then it represents
   * null value.
   */
  using value = std::shared_ptr<internal::base>;

  /**
   * Representation of JSON array.
   */
  class array final : public internal::base
  {
  public:
    using value_type = value;
    using container_type = std::vector<value_type>;

    array(const container_type& elements = container_type())
      : m_elements(elements) {}

    array(std::initializer_list<value_type> init)
      : m_elements(init) {}

    static std::shared_ptr<array> make(std::initializer_list<value_type> init)
    {
      return std::make_shared<array>(init);
    }

    inline enum type type() const
    {
      return type::array;
    }

    inline const container_type& elements() const
    {
      return m_elements;
    }

  private:
    const container_type m_elements;
  };

  /**
   * Representation of JSON boolean.
   */
  class boolean final : public internal::base
  {
  public:
    using value_type = bool;

    boolean(value_type value = false)
      : m_value(value) {}

    static std::shared_ptr<boolean> make(value_type value)
    {
      return std::make_shared<boolean>(value);
    }

    inline enum type type() const
    {
      return type::boolean;
    }

    inline value_type value() const
    {
      return m_value;
    }

  private:
    const value_type m_value;
  };

  /**
   * Representation of JSON number.
   */
  class number final : public internal::base
  {
  public:
    using value_type = double;

    number(value_type value = 0.0)
      : m_value(value) {}

    static std::shared_ptr<number> make(value_type value)
    {
      return std::make_shared<number>(value);
    }

    inline enum type type() const
    {
      return type::number;
    }

    inline value_type value() const
    {
      return m_value;
    }

  private:
    const value_type m_value;
  };

  /**
   * Representation of JSON object.
   */
  class object final : public internal::base
  {
  public:
    using key_type = std::u32string;
    using mapped_type = value;
    using container_type = std::unordered_map<key_type, mapped_type>;
    using value_type = container_type::value_type;

    object(const container_type& properties = container_type())
      : m_properties(properties) {}

    object(std::initializer_list<value_type> init)
      : m_properties(init) {}

    static std::shared_ptr<object> make(std::initializer_list<value_type> init)
    {
      return std::make_shared<object>(init);
    }

    inline enum type type() const
    {
      return type::object;
    }

    inline const container_type& properties() const
    {
      return m_properties;
    }

  private:
    const container_type m_properties;
  };

  /**
   * Representation of JSON string.
   */
  class string final : public internal::base
  {
  public:
    using value_type = std::u32string;

    string(const value_type& value = value_type())
      : m_value(value) {}

    static std::shared_ptr<string> make(const value_type& value)
    {
      return std::make_shared<string>(value);
    }

    inline enum type type() const
    {
      return type::string;
    }

    inline const value_type& value() const
    {
      return m_value;
    }

  private:
    const value_type m_value;
  };

  /**
   * Returns type of given value.
   */
  inline enum type
  type_of(const value& v)
  {
    return v ? v->type() : type::null;
  }

  /**
   * Shortcut for `std::static_pointer_cast` function. Notice that no type
   * checking is done, so you need to do that yourself first with `type_of`
   * function.
   */
  template<class T>
  inline std::shared_ptr<T>
  as(const value& v)
  {
    return std::static_pointer_cast<T>(v);
  }
}
