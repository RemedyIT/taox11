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

#include <sstream>
#include <cstdlib>
#include "iostream"
#include "ace/ace_wchar.h"

namespace std
{
  /// std::wstring to ostream insertion
  inline std::ostream&
  operator<< (std::ostream& _os, const std::wstring& _v)
  {
    return _os << "\"" << ACE_Wide_To_Ascii (_v.c_str ()).char_rep () << "\"";
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
