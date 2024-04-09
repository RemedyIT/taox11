/**
 * @file    stub_arg_traits.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 argument traits for client stubs
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_STUB_ARG_TRAITS_H
#define TAOX11_STUB_ARG_TRAITS_H

#pragma once

#include "tao/CORBA_methods.h"
#include "tao/x11/tao_corba.h"
#include "tao/x11/arg_traits_t.h"
#include "tao/x11/basic_argument_t.h"
#include "tao/x11/anyinsert_policy_t.h"
#include "tao/x11/object.h"

// --------------------------------------------------------------

namespace TAOX11_NAMESPACE
{
  /// Used in generated code if CORBA::Object is an argument or
  /// return type.
  template<>
  class Arg_Traits<TAOX11_CORBA::Object>
    : public Basic_Arg_Traits_T<
        TAOX11_CORBA::object_reference<TAOX11_CORBA::Object>,
        Any_Insert_Policy_AnyInsert_Adapter>
  {
  };
} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// CDR streaming operator specializations
TAOX11_Export TAO_CORBA::Boolean operator<< (TAO_OutputCDR &, TAOX11_CORBA::object_traits<TAOX11_CORBA::Object>::ref_type);
TAOX11_Export TAO_CORBA::Boolean operator>> (TAO_InputCDR &, TAOX11_CORBA::object_traits<TAOX11_CORBA::Object>::ref_type&);

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAOX11_STUB_ARG_TRAITS_H */
