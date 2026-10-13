/**
 * @file    test.cpp
 * @author  Johnny Willemsen
 * @brief   Standalone UTF conversion and stream formatting regression tests.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/base/wstringwchar_ostream.h"
#include "tao/x11/base/utf8_utf16.h"
#include "testlib/taox11_testlog.h"
#include <limits>
#include <sstream>

// Use the configured namespace without requiring generated IDL headers.
namespace x11 = TAOX11_VERSIONED_NAMESPACE_NAME;

namespace
{
  int errors = 0;

  void check (bool condition, char const* description)
  {
    if (!condition)
    {
      TAOX11_TEST_ERROR << "ERROR: " << description << std::endl;
      ++errors;
    }
  }

  template <typename Convert, typename Input>
  void check_invalid (Convert convert, Input input)
  {
    try
    {
      convert (input);
      check (false, "invalid input accepted");
    }
    catch (std::range_error const&)
    {
    }
  }
}

int main ()
{
  using x11::detail::utf8_to_utf16;
  using x11::detail::utf16_to_utf8;

  // Independent expected encodings, including every UTF-8 length boundary.
  std::string const utf8 = "A\xc3\xa9\xe2\x82\xac\xf0\x9f\x98\x80";
  std::wstring const wide {L'A', wchar_t (0xe9), wchar_t (0x20ac), wchar_t (0xd83d), wchar_t (0xde00)};
  check (utf8_to_utf16 (utf8) == wide, "UTF-8 decoding");
  check (utf16_to_utf8 (wide) == utf8, "UTF-16 encoding");
  check (utf8_to_utf16 ("").empty (), "empty UTF-8");
  check (utf16_to_utf8 ({}).empty (), "empty wide view");

  std::string const boundaries = "\x7f\xc2\x80\xdf\xbf\xe0\xa0\x80\xed\x9f\xbf\xee\x80\x80"
                                 "\xef\xbf\xbf\xf0\x90\x80\x80\xf4\x8f\xbf\xbf";
  std::wstring const boundary_wide {wchar_t (0x7f), wchar_t (0x80), wchar_t (0x7ff), wchar_t (0x800),
    wchar_t (0xd7ff), wchar_t (0xe000), wchar_t (0xffff), wchar_t (0xd800), wchar_t (0xdc00),
    wchar_t (0xdbff), wchar_t (0xdfff)};
  check (utf8_to_utf16 (boundaries) == boundary_wide, "UTF-8 boundaries");
  check (utf16_to_utf8 (boundary_wide) == boundaries, "UTF-16 boundaries");

  std::string const nul_bytes {"a\0b", 3};
  std::wstring const nul_wide {L"a\0b", 3};
  check (utf8_to_utf16 (nul_bytes) == nul_wide, "decode embedded NUL");
  check (utf16_to_utf8 (nul_wide) == nul_bytes, "encode embedded NUL");

  for (char const* invalid : {"\x80", "\xc0\x80", "\xc1\xbf", "\xc2", "\xc2X", "\xe0\x80\x80",
                             "\xed\xa0\x80", "\xf0\x80\x80\x80", "\xf4\x90\x80\x80", "\xf5\x80\x80\x80",
                             "\xff", "\xe2\x82", "\xf0\x9f\x98"})
    check_invalid (utf8_to_utf16, std::string_view (invalid));
  for (std::wstring const& invalid : {std::wstring {wchar_t (0xd800)}, std::wstring {wchar_t (0xdc00)},
       std::wstring {wchar_t (0xd800), L'x'}, std::wstring {wchar_t (0xd800), wchar_t (0xd800)}})
    check_invalid (utf16_to_utf8, std::wstring_view (invalid));

  if (sizeof (wchar_t) > 2)
  {
    check (utf16_to_utf8 (std::wstring {wchar_t (0x1f600)}) == "\xf0\x9f\x98\x80",
           "32-bit wchar_t scalar value");
    check_invalid (utf16_to_utf8, std::wstring_view (std::wstring {wchar_t (0x110000)}));
  }
  if (std::numeric_limits<wchar_t>::is_signed)
    check_invalid (utf16_to_utf8, std::wstring_view (std::wstring {wchar_t (-1)}));

  std::ostringstream stream;
  stream << wide;
  check (stream.str () == '"' + utf8 + '"', "wstring insertion");
  stream.str ("");
  stream << std::wstring_view (wide);
  check (stream.str () == '"' + utf8 + '"', "wstring_view insertion");

  // The buffer is not NUL-terminated. The second character lies outside the view.
  wchar_t const buffer[] {L'a', L'b'};
  stream.str ("");
  stream << std::wstring_view (buffer, 1);
  check (stream.str () == "\"a\"", "view length respected");
  stream.str ("");
  stream << std::wstring_view {};
  check (stream.str () == "\"\"", "default empty view insertion");
  stream.str ("");
  stream << std::wstring_view (nul_wide);
  check (stream.str () == '"' + nul_bytes + '"', "view with embedded NUL");

  return errors == 0 ? 0 : 1;
}
