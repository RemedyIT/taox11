/**
 * @file    wstringwchar_ostream.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_WTRINGWCHAR_OSTREAM_H
#define TAOX11_WTRINGWCHAR_OSTREAM_H

#include <ostream>
#include <cstdlib>
#include <codecvt>
#include <locale>

namespace std
{
  /// std::wstring to ostream insertion
  inline std::ostream&
  operator<< (std::ostream& _os, const std::wstring& _v)
  {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return _os << "\"" << conv.to_bytes(_v) << "\"";
  }

  /// wchar_t to ostream insertion
  inline std::ostream&
  operator<< (std::ostream& _os, const wchar_t& _v)
  {
    // Thread safe conversion
    std::string mb(MB_CUR_MAX, '\0');
    std::mbstate_t state = std::mbstate_t ();
    std::size_t const ret = std::wcrtomb(std::addressof(mb[0]), _v, std::addressof(state));
    if (ret > 0)
      {
        return _os << "'" << mb << "'";
      }
    else
      {
        return _os << "''";
      }
  }
}

#endif /* TAOX11_WTRINGWCHAR_OSTREAM_H */
