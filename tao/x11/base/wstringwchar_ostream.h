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
#include <string_view>
#if !defined (WIN32)
# include <codecvt>
# include <locale>
#endif

namespace std
{
  /// std::wstring to ostream insertion
  inline std::ostream&
  operator<< (std::ostream& _os, const std::wstring& _v)
  {
#if defined (WIN32)
    std::string _str{};
    if (!_v.empty ())
    {
      int const size_needed = WideCharToMultiByte (CP_UTF8, 0, _v.data (), static_cast<int>(_v.size ()), nullptr, 0, nullptr, nullptr);
      _str.resize (size_needed);
      WideCharToMultiByte (CP_UTF8, 0, _v.data (), static_cast<int>(_v.size ()), _str.data (), size_needed, nullptr, nullptr);
    }
    return _os << "\"" << _str << "\"";
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return _os << "\"" << conv.to_bytes(_v) << "\"";
#endif
  }

  /// std::wstring_view to ostream insertion
  inline std::ostream&
  operator<< (std::ostream& _os, const std::wstring_view& _v)
  {
#if defined (WIN32)
    std::string _str {};
    if (!_v.empty ())
    {
      int const size_needed = WideCharToMultiByte (CP_UTF8, 0, _v.data (), static_cast<int>(_v.size ()), nullptr, 0, nullptr, nullptr);
      _str.resize(size_needed);
      WideCharToMultiByte (CP_UTF8, 0, _v.data (), static_cast<int>(_v.size ()), _str.data (), size_needed, nullptr, nullptr);
    }
    return _os << "\"" << _str << "\"";
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return _os << "\"" << conv.to_bytes(_v.data()) << "\"";
#endif
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
