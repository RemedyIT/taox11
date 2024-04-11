/**
 * @file    any_arg_traits.h
 * @author  Marijke Hengstmengel
 *
 * @brief   TAOX11 argument traits for CORBA::Any
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANYTYPECODE_ANY_ARG_TRAITS_H
#define TAOX11_ANYTYPECODE_ANY_ARG_TRAITS_H

#pragma once

#include "tao/CORBA_methods.h"
#include "tao/x11/basic_argument_t.h"

#include "tao/x11/base/tao_corba.h"

#include "tao/x11/arg_traits_t.h"
#include "tao/x11/anyinsert_policy_t.h"

#include "tao/x11/anytypecode/any.h"

// --------------------------------------------------------------

namespace TAOX11_NAMESPACE
{
  /// Used in generated code if CORBA::Any is an argument or
  /// return type.
  template<>
  class Arg_Traits<TAOX11_CORBA::Any>
    : public Basic_Arg_Traits_T<TAOX11_CORBA::Any,
                                Any_Insert_Policy_Stream>
  {
  };
} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// CDR streaming operator specializations
TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean operator<< (TAO_OutputCDR &, const TAOX11_CORBA::Any& );
TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean operator>> (TAO_InputCDR &, TAOX11_CORBA::Any&);

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAOX11_ANYTYPECODE_ANY_ARG_TRAITS_H */
