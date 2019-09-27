/**
 * @file    abstractbase_proxy.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 AbstractBase TAO proxy classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/valuetype/abstractbase_proxy.h"

namespace TAOX11_NAMESPACE
{

  Abstractbase_proxy::Abstractbase_proxy (
      TAO_Stub *p,
      TAO_CORBA::Boolean f,
      TAO_Abstract_ServantBase *s)
    : TAO_CORBA::AbstractBase (p, f, s)
  {}
} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBA::Boolean
operator<< (TAO_OutputCDR &strm, TAOX11_CORBA::AbstractBase::_ref_type abs)
{
  return TAOX11_CORBA::AbstractBase::_abs_marshal (strm, std::move(abs));
}

TAO_CORBA::Boolean
operator>> (TAO_InputCDR &strm, TAOX11_CORBA::AbstractBase::_ref_type &abs)
{
  return TAOX11_CORBA::AbstractBase::_abs_unmarshal (strm, abs);
}

TAO_END_VERSIONED_NAMESPACE_DECL
