// -*- C++ -*-
/**
 * @file fixed_t.h
 * @author Johnny Willemsen
 * @brief IDL fixed decimal value, backed by ACE_CDR::Fixed
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef __IDL_FIXED_T_H_INCLUDED__
#define __IDL_FIXED_T_H_INCLUDED__

#include "ace/CDR_Base.h"
#include "tao/x11/base/versioned_x11_namespace.h"
#include "tao/x11/system_exception.h"

#include <cmath>
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
      explicit Fixed(std::string const& value) : value_(parse(value)) {}

      Fixed(Fixed const&) = default;
      Fixed(Fixed&&) = default;
      Fixed& operator=(Fixed const&) = default;
      Fixed& operator=(Fixed&&) = default;
      ~Fixed() = default;

      explicit operator int64_t() const
      {
        std::string const value = this->to_string();
        try
        {
          return std::stoll(value.substr(0, value.find('.')));
        }
        catch (std::out_of_range const&)
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

      Fixed& operator+=(Fixed const& rhs)
      {
        try { this->value_ = checked((this->value_ + rhs.value_).truncate(scale)); }
        catch (std::overflow_error const&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator-=(Fixed const& rhs)
      {
        try { this->value_ = checked((this->value_ - rhs.value_).truncate(scale)); }
        catch (std::overflow_error const&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator*=(Fixed const& rhs)
      {
        try { this->value_ = checked((this->value_ * rhs.value_).truncate(scale)); }
        catch (std::overflow_error const&) { throw CORBA::DATA_CONVERSION(); }
        return *this;
      }

      Fixed& operator/=(Fixed const& rhs)
      {
        if (!rhs)
          throw CORBA::DATA_CONVERSION();
        try { this->value_ = divide(this->value_, rhs.value_); }
        catch (std::overflow_error const&) { throw CORBA::DATA_CONVERSION(); }
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
        std::string const value = this->to_string();
        size_t const start = value.front() == '-' ? 1 : 0;
        size_t const point = value.find('.');
        size_t const end = point == std::string::npos ? value.size() : point;
        size_t first = start;
        while (first < end && value[first] == '0')
          ++first;
        size_t const used = end - first +
            (point == std::string::npos ? 0 : value.size() - point - 1);
        return static_cast<uint16_t>(used ? used : 1);
      }
      uint16_t fixed_scale() const { return this->value_.fixed_scale(); }

      friend Fixed operator+(Fixed lhs, Fixed const& rhs) { return lhs += rhs; }
      friend Fixed operator-(Fixed lhs, Fixed const& rhs) { return lhs -= rhs; }
      friend Fixed operator*(Fixed lhs, Fixed const& rhs) { return lhs *= rhs; }
      friend Fixed operator/(Fixed lhs, Fixed const& rhs) { return lhs /= rhs; }

      friend bool operator==(Fixed const& lhs, Fixed const& rhs) { return lhs.value_ == rhs.value_; }
      friend bool operator!=(Fixed const& lhs, Fixed const& rhs) { return !(lhs == rhs); }
      friend bool operator<(Fixed const& lhs, Fixed const& rhs) { return lhs.value_ < rhs.value_; }
      friend bool operator>(Fixed const& lhs, Fixed const& rhs) { return rhs < lhs; }
      friend bool operator<=(Fixed const& lhs, Fixed const& rhs) { return !(rhs < lhs); }
      friend bool operator>=(Fixed const& lhs, Fixed const& rhs) { return !(lhs < rhs); }

      friend void swap(Fixed& lhs, Fixed& rhs)
      {
        using std::swap;
        swap(lhs.value_, rhs.value_);
      }

      friend std::istream& operator>>(std::istream& is, Fixed& value)
      {
        std::string token;
        if (is >> token)
        {
          try { value = Fixed(token); }
          catch (CORBA::DATA_CONVERSION const&) { is.setstate(std::ios::failbit); }
        }
        return is;
      }

    private:
      explicit Fixed(ACE_CDR::Fixed const& value) : value_(checked(value)) {}

      static ACE_CDR::Fixed checked(ACE_CDR::Fixed const& value)
      {
        char buffer[ACE_CDR::Fixed::MAX_STRING_SIZE];
        if (!value.to_string(buffer, sizeof(buffer)))
          throw CORBA::DATA_CONVERSION();
        return parse(buffer);
      }

      static std::string normalize_digits(std::string value)
      {
        const size_t first = value.find_first_not_of('0');
        if (first == std::string::npos)
          return "0";
        value.erase(0, first);
        return value;
      }

      static int compare_digits(const std::string& lhs, const std::string& rhs)
      {
        if (lhs.size() != rhs.size())
          return lhs.size() < rhs.size() ? -1 : 1;
        return lhs.compare(rhs);
      }

      static std::string subtract_digits(const std::string& lhs, const std::string& rhs)
      {
        std::string result(lhs.size(), '0');
        int borrow = 0;
        for (size_t i = 0; i < lhs.size(); ++i)
        {
          int digit = lhs[lhs.size() - i - 1] - '0' - borrow;
          if (i < rhs.size())
            digit -= rhs[rhs.size() - i - 1] - '0';
          if (digit < 0)
          {
            digit += 10;
            borrow = 1;
          }
          else
            borrow = 0;
          result[lhs.size() - i - 1] = static_cast<char>('0' + digit);
        }
        return normalize_digits(result);
      }

      static std::string divide_digits(const std::string& numerator,
                                       const std::string& denominator)
      {
        std::string quotient;
        std::string remainder("0");
        for (const char digit : numerator)
        {
          remainder = normalize_digits(remainder + digit);
          unsigned int quotient_digit = 0;
          while (compare_digits(remainder, denominator) >= 0)
          {
            remainder = subtract_digits(remainder, denominator);
            ++quotient_digit;
          }
          quotient += static_cast<char>('0' + quotient_digit);
        }
        return normalize_digits(quotient);
      }

      static std::string fixed_digits(const ACE_CDR::Fixed& value,
                                      uint16_t& value_scale, bool& negative)
      {
        char buffer[ACE_CDR::Fixed::MAX_STRING_SIZE];
        if (!value.to_string(buffer, sizeof(buffer)))
          throw CORBA::DATA_CONVERSION();

        std::string number(buffer);
        negative = number.front() == '-';
        if (negative)
          number.erase(0, 1);
        const size_t point = number.find('.');
        value_scale = point == std::string::npos
            ? 0 : static_cast<uint16_t>(number.size() - point - 1);
        if (point != std::string::npos)
          number.erase(point, 1);
        return normalize_digits(number);
      }

      static ACE_CDR::Fixed divide(const ACE_CDR::Fixed& lhs,
                                   const ACE_CDR::Fixed& rhs)
      {
        uint16_t lhs_scale = 0;
        uint16_t rhs_scale = 0;
        bool lhs_negative = false;
        bool rhs_negative = false;
        std::string numerator = fixed_digits(lhs, lhs_scale, lhs_negative);
        std::string denominator = fixed_digits(rhs, rhs_scale, rhs_negative);
        numerator.append(rhs_scale + scale, '0');
        denominator.append(lhs_scale, '0');

        std::string quotient = divide_digits(numerator, denominator);
        if (scale)
        {
          if (quotient.size() <= scale)
            quotient.insert(0, scale + 1 - quotient.size(), '0');
          quotient.insert(quotient.size() - scale, 1, '.');
        }
        if (lhs_negative != rhs_negative && quotient.find_first_of("123456789") != std::string::npos)
          quotient.insert(0, 1, '-');
        return parse(quotient);
      }

      static ACE_CDR::Fixed parse(std::string text)
      {
        if (text.empty())
          throw CORBA::DATA_CONVERSION();
        if (text.back() == 'd' || text.back() == 'D')
          text.pop_back();

        size_t const start = (!text.empty() && (text.front() == '-' || text.front() == '+')) ? 1 : 0;
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
        size_t const decimal = point == std::string::npos ? text.size() : point;
        size_t const first = text.find_first_not_of('0', start);
        size_t const integral = first != std::string::npos && first < decimal ? decimal - first : 0;
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

namespace std
{
  template <uint16_t digits, uint16_t scale>
  std::string to_string(TAOX11_NAMESPACE::IDL::Fixed<digits, scale> const& value)
  {
    return value.to_string();
  }
}

#endif // __IDL_FIXED_T_H_INCLUDED__
