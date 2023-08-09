/**
 * @file    portableserver_functions.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/portable_server/portableserver_functions.h"

#include <string>
#include <sstream>

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    std::string
    ObjectId_to_string (const TAOX11_NAMESPACE::PortableServer::ObjectId &id)
    {
      std::string s (id.cbegin(), id.cend());
      return s;
    }

    std::wstring
    ObjectId_to_wstring (const TAOX11_NAMESPACE::PortableServer::ObjectId &id)
    {
      std::string s (id.cbegin(), id.cend());
      std::wstring ws (ACE_Ascii_To_Wide (s.c_str ()).wchar_rep ());
      return ws;
    }

    TAOX11_NAMESPACE::PortableServer::ObjectId
    string_to_ObjectId (const std::string& string)
    {
      TAOX11_NAMESPACE::PortableServer::ObjectId id (string.cbegin(), string.cend());

      return id;
    }

    TAOX11_NAMESPACE::PortableServer::ObjectId
    wstring_to_ObjectId (const std::wstring& wstring)
    {
      std::ostringstream s_out;
      s_out << ACE_Wide_To_Ascii (wstring.c_str ()).char_rep ();
      std::string string = s_out.str ();

      TAOX11_NAMESPACE::PortableServer::ObjectId id (string.cbegin(), string.cend());

      return id;
    }
  }
}
