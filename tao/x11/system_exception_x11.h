/**
 * @file    system_exception_x11.h
 * @author  Martin Corino
 *
 * @brief   Converter for TAO to TAOX11
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SYSTEM_EXCEPTION_X11_H_INCLUDED
#define TAOX11_SYSTEM_EXCEPTION_X11_H_INCLUDED

#include "tao/x11/system_exception.h"
#include /**/ "tao/Versioned_Namespace.h"

namespace TAO_VERSIONED_NAMESPACE_NAME
{
  namespace CORBA
  {
    class SystemException; // forward
  }
}

namespace TAOX11_NAMESPACE
{
  TAOX11_Export void _raise_x11 (const TAO_VERSIONED_NAMESPACE_NAME::CORBA::SystemException&);
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_SYSTEM_EXCEPTION_X11_H_INCLUDED
