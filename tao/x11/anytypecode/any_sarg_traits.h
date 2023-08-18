/**
 * @file    any_sarg_traits.h
 * @author  Marijke Hengstmengel
 *
 * @brief   x11 skeleton argument traits template
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANYTYPECODE_ANY_SARG_TRAITS_H
#define TAOX11_ANYTYPECODE_ANY_SARG_TRAITS_H

#pragma once

#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/any_arg_traits.h"

#include "tao/x11/portable_server/sarg_traits_t.h"
#include "tao/x11/portable_server/basic_sargument_t.h"

#include "tao/x11/object.h"

namespace TAOX11_NAMESPACE::PS
{
  /// Used in generated code if CORBA::Any is an argument or
  /// return type.
  template<>
  class SArg_Traits<TAOX11_NAMESPACE::CORBA::Any>
    : public Basic_SArg_Traits_T<TAOX11_NAMESPACE::CORBA::Any,
                                  Any_Insert_Policy_Stream>
  {
  };
} // namespace TAOX11_NAMESPACE::PS

#endif /* TAOX11_ANYTYPECODE_ANY_SARG_TRAITS_T_H */
