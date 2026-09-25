// -*- C++ -*-
/**
 * @file fixed_t.h
 * @brief IDL fixed decimal value, backed by ACE_CDR::Fixed
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef __IDL_FIXED_T_H_INCLUDED__
#define __IDL_FIXED_T_H_INCLUDED__

#include "ace/CDR_Base.h"
#include "tao/CDR.h"
#include "tao/x11/base/versioned_x11_namespace.h"
#include "tao/x11/system_exception.h"

#include <cmath>
#include <array>
#include <cstdint>
#include <iomanip>
#include <istream>
#include <locale>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template <uint16_t digits, uint16_t scale>
    class Fixed final
    {
      static_assert(digits > 0 && digits <= ACE_CDR::Fixed::MAX_DIGITS,
                    "fixed digits must be between 1 and 31");
      static_assert(scale <= digits, "fixed scale must not exceed digits");

    public:
      explicit Fixed(int16_t value = 0) : Fixed(std::to_string(value)) {}
      explicit Fixed(uint16_t value) : Fixed(std::to_string(value)) {}
      explicit Fixed(int32_t value) : Fixed(std::to_string(value)) {}
      explicit Fixed(uint32_t value) : Fixed(std::to_string(value)) {}
      explicit Fixed(int64_t value) : Fixed(std::to_string(value)) {}
      explicit Fixed(uint64_t value) : Fixed(std::to_string(value)) {}
      explicit Fixed(double value) : Fixed(from_floating(value)) {}
      explicit Fixed(long double value) : Fixed(from_floating(value)) {}
      explicit Fixed(const std::string& value) : value_(parse(value)) {}

      Fixed(const Fixed&) = default;
      Fixed(Fixed&&) = default;
      Fixed& operator=(const Fixed&) = default;
      Fixed& operator=(Fixed&&) = default;
      ~Fixed() = default;

      explicit operator int64_t() const
      {
        const std::string value = this->to_string();
        try
        {
          return std::stoll(value.substr(0, value.find('.')));
        }
        catch (const std::out_of_range&)
        {
          throw CORBA::DATA_CONVERSION();
        }
      }

      explicit operator long double() const
      {
        return static_cast<ACE_CDR::LongDouble>(this->value_);
      }

      Fixed round(uint16_t new_scale) const
      {
        if (new_scale >= scale)
          return *this;
        return Fixed(this->value_.round(new_scale));
      }

      Fixed truncate(uint16_t new_scale) const
      {
        if (new_scale >= scale)
          return *this;
        return Fixed(this->value_.truncate(new_scale));
      }

      std::string to_string() const
      {
        char buffer[ACE_CDR::Fixed::MAX_STRING_SIZE];
        if (!this->value_.to_string(buffer, sizeof(buffer)))
          throw CORBA::DATA_CONVERSION();
        return buffer;
      }

      Fixed& operator+=(const Fixed& rhs)
      {
        try { this->value_ = checked(this->value_ + rhs.value_); }
        catch (const std::overflow_error&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator-=(const Fixed& rhs)
      {
        try { this->value_ = checked(this->value_ - rhs.value_); }
        catch (const std::overflow_error&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator*=(const Fixed& rhs)
      {
        try { this->value_ = checked(this->value_ * rhs.value_); }
        catch (const std::overflow_error&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator/=(const Fixed& rhs)
      {
        if (!rhs)
          throw CORBA::DATA_CONVERSION();
        try { this->value_ = checked(this->value_ / rhs.value_); }
        catch (const std::overflow_error&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator++() { return *this += Fixed(1); }
      Fixed operator++(int) { Fixed old(*this); ++*this; return old; }
      Fixed& operator--() { return *this -= Fixed(1); }
      Fixed operator--(int) { Fixed old(*this); --*this; return old; }

      Fixed operator+() const { return *this; }
      Fixed operator-() const { return Fixed(checked(-this->value_)); }
      explicit operator bool() const { return !(!this->value_); }

      uint16_t fixed_digits() const
      {
        const std::string value = this->to_string();
        const size_t start = value.front() == '-' ? 1 : 0;
        const size_t point = value.find('.');
        const size_t end = point == std::string::npos ? value.size() : point;
        size_t first = start;
        while (first < end && value[first] == '0')
          ++first;
        const size_t used = end - first +
            (point == std::string::npos ? 0 : value.size() - point - 1);
        return static_cast<uint16_t>(used ? used : 1);
      }
      uint16_t fixed_scale() const { return this->value_.fixed_scale(); }

      friend Fixed operator+(Fixed lhs, const Fixed& rhs) { return lhs += rhs; }
      friend Fixed operator-(Fixed lhs, const Fixed& rhs) { return lhs -= rhs; }
      friend Fixed operator*(Fixed lhs, const Fixed& rhs) { return lhs *= rhs; }
      friend Fixed operator/(Fixed lhs, const Fixed& rhs) { return lhs /= rhs; }

      friend bool operator==(const Fixed& lhs, const Fixed& rhs) { return lhs.value_ == rhs.value_; }
      friend bool operator!=(const Fixed& lhs, const Fixed& rhs) { return !(lhs == rhs); }
      friend bool operator<(const Fixed& lhs, const Fixed& rhs) { return lhs.value_ < rhs.value_; }
      friend bool operator>(const Fixed& lhs, const Fixed& rhs) { return rhs < lhs; }
      friend bool operator<=(const Fixed& lhs, const Fixed& rhs) { return !(rhs < lhs); }
      friend bool operator>=(const Fixed& lhs, const Fixed& rhs) { return !(lhs < rhs); }

      friend void swap(Fixed& lhs, Fixed& rhs)
      {
        using std::swap;
        swap(lhs.value_, rhs.value_);
      }

      friend std::ostream& operator<<(std::ostream& os, const Fixed& value)
      {
        return os << value.to_string();
      }

      friend std::istream& operator>>(std::istream& is, Fixed& value)
      {
        std::string token;
        if (is >> token)
        {
          try { value = Fixed(token); }
          catch (const CORBA::DATA_CONVERSION&) { is.setstate(std::ios::failbit); }
        }
        return is;
      }

      // The wire format always contains the declared number of decimal
      // digits, including leading zero digits and a sign nibble.
      template <typename Stream>
      bool write_cdr(Stream& cdr) const
      {
        std::string number = this->to_string();
        const bool negative = number.front() == '-';
        if (negative)
          number.erase(0, 1);
        const size_t point = number.find('.');
        if (point != std::string::npos)
          number.erase(point, 1);
        while (number.size() > digits && number.front() == '0')
          number.erase(0, 1);
        number.insert(0, digits - number.size(), '0');
        if (digits % 2 == 0)
          number.insert(0, 1, '0');

        std::array<ACE_CDR::Octet, (digits + 2) / 2> octets {};
        for (size_t i = 0; i + 1 < number.size(); i += 2)
          octets[i / 2] = static_cast<ACE_CDR::Octet>(
              ((number[i] - '0') << 4) | (number[i + 1] - '0'));
        octets.back() = static_cast<ACE_CDR::Octet>(
            ((number.back() - '0') << 4) | (negative ? 0x0d : 0x0c));
        return cdr.write_octet_array(octets.data(), octets.size());
      }

      template <typename Stream>
      static bool read_cdr(Stream& cdr, Fixed& value)
      {
        std::array<ACE_CDR::Octet, (digits + 2) / 2> octets {};
        if (!cdr.read_octet_array(octets.data(), octets.size()))
          return false;
        if (digits % 2 == 0 && (octets.front() >> 4) != 0)
          return false;
        const ACE_CDR::Octet sign = octets.back() & 0x0f;
        if (sign != 0x0c && sign != 0x0d)
          return false;

        std::string number;
        number.reserve(digits);
        for (size_t i = 0; i < octets.size(); ++i)
        {
          const ACE_CDR::Octet high = octets[i] >> 4;
          const ACE_CDR::Octet low = octets[i] & 0x0f;
          if (high > 9 || (i + 1 < octets.size() && low > 9))
            return false;
          number += static_cast<char>('0' + high);
          if (i + 1 < octets.size())
            number += static_cast<char>('0' + low);
        }
        if (digits % 2 == 0)
          number.erase(0, 1);
        if (scale)
          number.insert(digits - scale, ".");
        if (sign == 0x0d)
          number.insert(0, "-");
        try
        {
          value = Fixed(number);
          return true;
        }
        catch (const CORBA::DATA_CONVERSION&)
        {
          return false;
        }
      }

    private:
      explicit Fixed(const ACE_CDR::Fixed& value) : value_(checked(value)) {}

      static ACE_CDR::Fixed checked(const ACE_CDR::Fixed& value)
      {
        char buffer[ACE_CDR::Fixed::MAX_STRING_SIZE];
        if (!value.to_string(buffer, sizeof(buffer)))
          throw CORBA::DATA_CONVERSION();
        return parse(buffer);
      }

      static ACE_CDR::Fixed parse(std::string text)
      {
        if (text.empty())
          throw CORBA::DATA_CONVERSION();
        if (text.back() == 'd' || text.back() == 'D')
          text.pop_back();

        const size_t start = (!text.empty() && (text.front() == '-' || text.front() == '+')) ? 1 : 0;
        size_t point = std::string::npos;
        size_t count = 0;
        for (size_t i = start; i < text.size(); ++i)
        {
          if (text[i] == '.' && point == std::string::npos)
            point = i;
          else if (text[i] >= '0' && text[i] <= '9')
            ++count;
          else
            throw CORBA::DATA_CONVERSION();
        }
        if (count == 0)
          throw CORBA::DATA_CONVERSION();
        const size_t decimal = point == std::string::npos ? text.size() : point;
        const size_t first = text.find_first_not_of('0', start);
        const size_t integral = first != std::string::npos && first < decimal ? decimal - first : 0;
        size_t fraction = point == std::string::npos ? 0 : text.size() - point - 1;
        if (fraction > scale)
        {
          for (size_t i = point + 1 + scale; i < text.size(); ++i)
            if (text[i] != '0')
              throw CORBA::DATA_CONVERSION();
          text.erase(point + 1 + scale);
          fraction = scale;
        }
        if (integral + scale > digits)
          throw CORBA::DATA_CONVERSION();
        if (point == std::string::npos && scale)
          text += '.';
        text.append(scale - fraction, '0');
        if (text.front() == '-' && text.find_first_of("123456789") == std::string::npos)
          text.erase(0, 1);
        return ACE_CDR::Fixed::from_string(text.c_str());
      }

      template <typename T>
      static std::string from_floating(T value)
      {
        if (!std::isfinite(value))
          throw CORBA::DATA_CONVERSION();
        std::ostringstream os;
        os.imbue(std::locale::classic());
        os << std::fixed << std::setprecision(scale) << value;
        return os.str();
      }

      ACE_CDR::Fixed value_;
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
  template <uint16_t digits, uint16_t scale>
  inline bool operator<<(TAO_OutputCDR& cdr, const TAOX11_NAMESPACE::IDL::Fixed<digits, scale>& value)
  {
    return value.write_cdr(cdr);
  }

  template <uint16_t digits, uint16_t scale>
  inline bool operator>>(TAO_InputCDR& cdr, TAOX11_NAMESPACE::IDL::Fixed<digits, scale>& value)
  {
    return TAOX11_NAMESPACE::IDL::Fixed<digits, scale>::read_cdr(cdr, value);
  }
TAO_END_VERSIONED_NAMESPACE_DECL

namespace std
{
  template <uint16_t digits, uint16_t scale>
  std::string to_string(const TAOX11_NAMESPACE::IDL::Fixed<digits, scale>& value)
  {
    return value.to_string();
  }
}

#endif // __IDL_FIXED_T_H_INCLUDED__
