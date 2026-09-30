/**
 * @file    utf8_utf16.h
 * @author  Johnny Willemsen
 * @brief   Locale-independent UTF-8/UTF-16 conversion for stream formatting.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_UTF8_UTF16_H_INCLUDED
#define TAOX11_UTF8_UTF16_H_INCLUDED

#include "tao/x11/base/versioned_x11_namespace.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace TAOX11_NAMESPACE
{
  namespace detail
  {
    // Preserve codecvt_utf8_utf16<wchar_t>'s UTF-16 output, including on
    // platforms with a 32-bit wchar_t. Reject malformed or incomplete input.
    inline std::wstring
    utf8_to_utf16 (std::string_view input)
    {
      std::wstring output;
      for (std::size_t pos = 0; pos < input.size ();)
      {
        auto const first = static_cast<unsigned char> (input[pos++]);
        uint32_t value = first;
        unsigned int remaining = 0;
        uint32_t minimum = 0;
        if (first >= 0xc2 && first <= 0xdf)
        {
          value = first & 0x1f;
          remaining = 1;
          minimum = 0x80;
        }
        else if (first >= 0xe0 && first <= 0xef)
        {
          value = first & 0x0f;
          remaining = 2;
          minimum = 0x800;
        }
        else if (first >= 0xf0 && first <= 0xf4)
        {
          value = first & 0x07;
          remaining = 3;
          minimum = 0x10000;
        }
        else if (first >= 0x80)
        {
          throw std::range_error ("Invalid UTF-8 input");
        }

        if (remaining > input.size () - pos)
          throw std::range_error ("Incomplete UTF-8 input");
        while (remaining > 0)
        {
          --remaining;
          auto const next = static_cast<unsigned char> (input[pos++]);
          if ((next & 0xc0) != 0x80)
            throw std::range_error ("Invalid UTF-8 continuation byte");
          value = (value << 6) | (next & 0x3f);
        }
        if (value < minimum || value > 0x10ffff || (value >= 0xd800 && value <= 0xdfff))
          throw std::range_error ("Invalid UTF-8 code point");

        if (value <= 0xffff)
          output.push_back (static_cast<wchar_t> (value));
        else
        {
          value -= 0x10000;
          output.push_back (static_cast<wchar_t> (0xd800 + (value >> 10)));
          output.push_back (static_cast<wchar_t> (0xdc00 + (value & 0x3ff)));
        }
      }
      return output;
    }

    // Also accept full Unicode scalar values when wchar_t is 32 bits, as
    // codecvt_utf8_utf16<wchar_t> does. Lengths include embedded NULs.
    inline std::string
    utf16_to_utf8 (std::wstring_view input)
    {
      std::string output;
      for (std::size_t pos = 0; pos < input.size (); ++pos)
      {
        uint32_t value = static_cast<uint32_t> (input[pos]);
        if (value >= 0xd800 && value <= 0xdbff)
        {
          if (++pos == input.size ())
            throw std::range_error ("Incomplete UTF-16 surrogate pair");
          uint32_t const low = static_cast<uint32_t> (input[pos]);
          if (low < 0xdc00 || low > 0xdfff)
            throw std::range_error ("Invalid UTF-16 surrogate pair");
          value = 0x10000 + ((value - 0xd800) << 10) + (low - 0xdc00);
        }
        else if (value > 0x10ffff || (value >= 0xdc00 && value <= 0xdfff))
          throw std::range_error ("Invalid wide code point");

        if (value <= 0x7f)
          output.push_back (static_cast<char> (value));
        else if (value <= 0x7ff)
        {
          output.push_back (static_cast<char> (0xc0 | (value >> 6)));
          output.push_back (static_cast<char> (0x80 | (value & 0x3f)));
        }
        else if (value <= 0xffff)
        {
          output.push_back (static_cast<char> (0xe0 | (value >> 12)));
          output.push_back (static_cast<char> (0x80 | ((value >> 6) & 0x3f)));
          output.push_back (static_cast<char> (0x80 | (value & 0x3f)));
        }
        else
        {
          output.push_back (static_cast<char> (0xf0 | (value >> 18)));
          output.push_back (static_cast<char> (0x80 | ((value >> 12) & 0x3f)));
          output.push_back (static_cast<char> (0x80 | ((value >> 6) & 0x3f)));
          output.push_back (static_cast<char> (0x80 | (value & 0x3f)));
        }
      }
      return output;
    }
  }
}

#endif /* TAOX11_UTF8_UTF16_H_INCLUDED */
