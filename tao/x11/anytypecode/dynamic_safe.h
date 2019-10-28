/**
 * @file    dynamic_safe.h
 * @author  Martin Corino
 *
 * @brief   Safe include header for DynamicC.h to prevent
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNAMIC_SAFE_H
#define TAOX11_DYNAMIC_SAFE_H

#pragma once

#include /**/ "ace/config-all.h"

#ifndef _INTF_TAOX11_NAMESPACE_CORBA_TYPECODE_CDR_OPS_
// Prevent visibility of CDR ops for TypeCode declared in Typecode_typesP.h
// since these have the wrong export flags (being defined in taox11 core lib).
#define _INTF_TAOX11_NAMESPACE_CORBA_TYPECODE_CDR_OPS_

#include "tao/x11/anytypecode/DynamicC.h"

// Provide right CDR ops declarations

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAOX11_AnyTypeCode_Export bool
operator<< (TAO_OutputCDR & cdr, TAOX11_CORBA::object_traits< TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type tc);

TAOX11_AnyTypeCode_Export bool
operator>> (TAO_InputCDR & cdr, TAOX11_CORBA::object_traits< TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type& tc);

// CDR streaming helpers for typecode ref sequence
template <>
TAOX11_AnyTypeCode_Export bool
marshal_sequence<void, TAO_OutputCDR, TAOX11_CORBA::object_traits< TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type> (
    TAO_OutputCDR& _strm,
    const std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound);
template <>
TAOX11_AnyTypeCode_Export bool
demarshal_sequence<void, TAO_InputCDR, TAOX11_CORBA::object_traits< TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type> (
    TAO_InputCDR& _strm,
    std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound);

template <typename CDR_traits>
inline bool
marshal_sequence (
    TAO_OutputCDR& _strm,
    const std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound)
{
  return marshal_sequence<void, TAO_OutputCDR, TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type> (
      _strm, _seq, _bound);
}
template <typename CDR_traits>
inline bool
demarshal_sequence (
    TAO_InputCDR& _strm,
    std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound)
{
  return demarshal_sequence<void, TAO_InputCDR, TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type> (
      _strm, _seq, _bound);
}

TAO_END_VERSIONED_NAMESPACE_DECL
#else
// If typecode CDR OPS have already been declared, just include DynamicC.h
#include "tao/x11/anytypecode/DynamicC.h"
#endif

#endif /* TAOX11_DYNAMIC_SAFE_H */
