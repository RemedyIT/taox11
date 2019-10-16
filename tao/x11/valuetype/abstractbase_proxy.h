/**
 * @file    abstractbase_proxy.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 AbstractBase TAO proxy classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUETYPE_ABSTRACTBASE_PROXY_H_INCLUDED
#define TAOX11_VALUETYPE_ABSTRACTBASE_PROXY_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/tao_corba.h"
#include "tao/Valuetype/AbstractBase.h"
#include "tao/x11/valuetype/taox11_valuetype_export.h"
#include "tao/x11/valuetype/abstract_base.h"

namespace TAOX11_NAMESPACE
{
  //-------------------------------------------------------------------
  //  Valuetype proxy class
  //
  //  Wrapper class for reference to TAOX11 Value instance to enable
  //  to marshal/demarshal to/from TAO CDR streams.
  //===================================================================

  class Abstractbase_proxy;
  typedef Abstractbase_proxy* Abstractbase_proxy_ptr;
  typedef TAO_Pseudo_Var_T<Abstractbase_proxy> Abstractbase_proxy_var;

  class TAOX11_Valuetype_Export Abstractbase_proxy
      : public TAO_CORBA::AbstractBase
  {
  public:
    typedef Abstractbase_proxy_ptr  _ptr_type;

    static _ptr_type _nil ()
    { return nullptr; }

  protected:
    friend class TAOX11_CORBA::AbstractBase;

    Abstractbase_proxy (TAO_Stub *p,
                        TAO_CORBA::Boolean,
                        TAO_Abstract_ServantBase *);

    virtual ~Abstractbase_proxy () = default;
  };

} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAOX11_Valuetype_Export TAO_CORBA::Boolean
operator<< (TAO_OutputCDR &, TAOX11_CORBA::AbstractBase::_ref_type);

TAOX11_Valuetype_Export TAO_CORBA::Boolean
operator>> (TAO_InputCDR &, TAOX11_CORBA::AbstractBase::_ref_type &);

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_VALUETYPE_ABSTRACTBASE_PROXY_H_INCLUDED
