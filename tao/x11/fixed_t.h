// -*- C++ -*-
/**
 * @file  fixed_t.h
 * @author Martin Corino
 *
 * @brief templated for IDL fixed type
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (__IDL_FIXED_T_H_INCLUDED__)
#define __IDL_FIXED_T_H_INCLUDED__

#include <string>
#include "tao/x11/versioned_x11_namespace.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template <uint16_t digits, uint16_t scale>
    class Fixed final
    {
    public:
      // Constructors
      explicit Fixed(int16_t = 0) {}
      explicit Fixed(uint16_t) {}
      explicit Fixed(int32_t) {}
      explicit Fixed(uint32_t) {}
      explicit Fixed(int64_t) {}
      explicit Fixed(uint64_t) {}
      explicit Fixed(double) {}
      explicit Fixed(long double) {}
      explicit Fixed(const std::string&) {}
      Fixed(const Fixed&) {}
      Fixed(Fixed&& ) {}
      ~Fixed() = default;
      // Conversions
      explicit operator int64_t () const { return 0; }
      explicit operator long double() const { return 0.0; }
      Fixed round(uint16_t) const { return *this; }
      Fixed truncate(uint16_t) const { return *this; }
      std::string to_string() const { return "Not implemented"; }
      // Operators
      Fixed& operator=(const Fixed&) { return *this; }
      Fixed& operator=(Fixed&&) { return *this; }
      Fixed& operator+=(const Fixed&) { return *this; }
      Fixed& operator-=(const Fixed&) { return *this; }
      Fixed& operator*=(const Fixed&) { return *this; }
      Fixed& operator/=(const Fixed&) { return *this; }
      Fixed& operator++() { return *this; }
      Fixed operator++(int) { return *this; }
      Fixed& operator--() { return *this; }
      Fixed operator--(int) { return *this; }
      Fixed operator+() const { return *this; }
      Fixed operator-() const { return *this; }
      explicit operator bool() const { return true; }
      // Other member functions
      uint16_t fixed_digits() const { return 0; }
      uint16_t fixed_scale() const { return 0; }
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

namespace std
{
  template <uint16_t digits, uint16_t scale>
  std::string to_string (const TAOX11_IDL::Fixed <digits, scale>&)
  {
    return "Not implemented";
  }
} // namespace std

#endif // __IDL_FIXED_T_H_INCLUDED__
