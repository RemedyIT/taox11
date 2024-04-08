/**
 * @file    stub_sarg_traits.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 argument traits for servant stubs
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_STUB_SARG_TRAITS_H
#define TAOX11_STUB_SARG_TRAITS_H

#pragma once

#include "tao/CORBA_methods.h"
#include "tao/Any_Insert_Policy_T.h"
#include "tao/x11/portable_server/sarg_traits_t.h"
#include "tao/x11/portable_server/basic_sargument_t.h"
#include "tao/x11/object.h"

namespace TAOX11_NAMESPACE::PS
{
  /// Used in generated code if CORBA::Object is an argument or
  /// return type.
  template<>
  class SArg_Traits<TAOX11_NAMESPACE::CORBA::Object>
    : public Basic_SArg_Traits_T<
          TAOX11_CORBA::object_reference<TAOX11_NAMESPACE::CORBA::Object>,
          Any_Insert_Policy_Stream>
  {
  };
} // namespace TAOX11_NAMESPACE::PS

#endif /* TAOX11_STUB_SARG_TRAITS_H */
