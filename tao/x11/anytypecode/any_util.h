/**
 * @file    any_util.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any utility methods
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_UTIL_H_INCLUDED
#define TAOX11_ANY_UTIL_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/base/tao_corba.h"
#include "tao/x11/base/stddef.h"

#include "tao/x11/anytypecode/taox11_anytypecode_export.h"
#include "tao/x11/anytypecode/any_arg_traits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Any; // forward
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class Any; // forward
  }

  class TAOX11_AnyTypeCode_Export Any_Util
  {
  public:
    static bool x11_any_to_tao_any (const CORBA::Any&, TAO_CORBA::Any&);

    static bool tao_any_to_x11_any (const TAO_CORBA::Any&, CORBA::Any&);
  }; // Any_Util

} // namespace TAOX11_NAMESPACE

#endif // TAOX11_ANY_UTIL_H_INCLUDED
