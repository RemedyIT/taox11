/**
 * @file    exceptionA.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA::Exception Any insertion ops
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/corba.h"

// Specializations for CORBA::Exception Any operators.
namespace TAOX11_NAMESPACE
{
  // Insertion of CORBA::Exception - copying.
  void
  operator<<= (CORBA::Any &any, const CORBA::Exception &exception)
  {
    exception._any_insert (any);
  }

  // Insertion of CORBA::Exception - non-copying.
  void
  operator<<= (CORBA::Any &any, CORBA::Exception &&exception)
  {
    exception._any_insert (any);
  }
} // namespace TAOX11_NAMESPACE
