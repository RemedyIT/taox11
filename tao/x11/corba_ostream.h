/**
 * @file    corba_ostream.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_CORBA_OSTREAM_H
#define TAOX11_CORBA_OSTREAM_H

#include "tao/x11/taox11_export.h"
#include "tao/x11/corba.h"

#include <sstream>
#include <cstdlib>
#include "iostream"

namespace std
{
  inline std::ostream&
        operator<< (std::ostream& _os,
                    TAOX11_CORBA::object_reference< ::TAOX11_NAMESPACE::CORBA::Object> /*_ve*/)
  {
    return _os << "CORBA:Object";
  }

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
    std::wcrtomb(std::addressof(mb[0]), _v, &state);
    return _os << "'" << mb << "'";
  }
}

#endif /* TAOX11_CORBA_OSTREAM_H */
