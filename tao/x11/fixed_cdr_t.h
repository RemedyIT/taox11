// -*- C++ -*-
/**
 * @file fixed_cdr_t.h
 * @author Johnny Willemsen
 * @brief CDR streaming for IDL fixed decimal values
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef __IDL_FIXED_CDR_T_H_INCLUDED__
#define __IDL_FIXED_CDR_T_H_INCLUDED__

#include "tao/x11/fixed_t.h"
#include "tao/x11/base/tao_corba.h"

#include <array>
#include <string>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <uint16_t digits, uint16_t scale>
struct taox11_fixed_cdr
{
  using fixed_type = TAOX11_NAMESPACE::IDL::Fixed<digits, scale>;

  // The wire format always contains the declared number of decimal
  // digits, including leading zero digits and a sign nibble.
  template <typename Stream>
  static bool insert(Stream& cdr, fixed_type const& value)
  {
    std::string number = value.to_string();
    bool const negative = number.front() == '-';
    if (negative)
      number.erase(0, 1);
    size_t const point = number.find('.');
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
  static bool extract(Stream& cdr, fixed_type& value)
  {
    std::array<ACE_CDR::Octet, (digits + 2) / 2> octets {};
    if (!cdr.read_octet_array(octets.data(), octets.size()))
      return false;
    if (digits % 2 == 0 && (octets.front() >> 4) != 0)
      return false;
    ACE_CDR::Octet const sign = octets.back() & 0x0f;
    if (sign != 0x0c && sign != 0x0d)
      return false;

    std::string number;
    number.reserve(digits);
    for (size_t i = 0; i < octets.size(); ++i)
    {
      ACE_CDR::Octet const high = octets[i] >> 4;
      ACE_CDR::Octet const low = octets[i] & 0x0f;
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
      value = fixed_type(number);
      return true;
    }
    catch (TAOX11_NAMESPACE::CORBA::DATA_CONVERSION const&)
    {
      return false;
    }
  }
};

template <uint16_t digits, uint16_t scale>
inline bool operator<<(TAO_OutputCDR& cdr, TAOX11_NAMESPACE::IDL::Fixed<digits, scale> const& value)
{
  return taox11_fixed_cdr<digits, scale>::insert(cdr, value);
}

template <uint16_t digits, uint16_t scale>
inline bool operator>>(TAO_InputCDR& cdr, TAOX11_NAMESPACE::IDL::Fixed<digits, scale>& value)
{
  return taox11_fixed_cdr<digits, scale>::extract(cdr, value);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // __IDL_FIXED_CDR_T_H_INCLUDED__
