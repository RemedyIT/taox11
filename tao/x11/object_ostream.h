/**
 * @file    object_ostream.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_OBJECT_OSTREAM_H
#define TAOX11_OBJECT_OSTREAM_H

#include "tao/x11/object_fwd.h"
#include <ostream>

namespace std
{
  inline std::ostream&
  operator<< (std::ostream& _os,
              TAOX11_CORBA::object_reference<::TAOX11_NAMESPACE::CORBA::Object> /*_ve*/)
  {
    return _os << "CORBA:Object";
  }
}

#endif /* TAOX11_OBJECT_OSTREAM_H */
