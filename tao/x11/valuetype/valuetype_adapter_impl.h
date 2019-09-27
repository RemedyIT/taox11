/**
 * @file    valuetype_adapter_impl.h
 * @author  Martin Corino
 *
 * @brief   Concrete valuetype adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_VALUETYPE_ADAPTER_IMPL_INCLUDED
#define TAOX11_VALUETYPE_ADAPTER_IMPL_INCLUDED

#pragma once

#include "ace/Service_Config.h"

#include "tao/x11/valuetype_adapter.h"
#include "tao/x11/valuetype/value_base.h"
#include "tao/x11/valuetype/abstract_base.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_Valuetype_Export ValueTypeAdapter_impl
    : public ValueTypeAdapter
  {
  public:
    ~ValueTypeAdapter_impl() = default;

    virtual bool
    stream_to_value (
        TAO_InputCDR &,
        CORBA::valuetype_reference<CORBA::ValueBase>&) override;

    virtual bool
    stream_to_abstract_base (
        TAO_InputCDR &,
        CORBA::abstractbase_reference<CORBA::AbstractBase> &) override;

    virtual bool
    value_to_stream (
        TAO_OutputCDR &,
        CORBA::valuetype_reference<CORBA::ValueBase>) override;

    virtual bool
    abstract_base_to_stream (
        TAO_OutputCDR &,
        CORBA::abstractbase_reference<CORBA::AbstractBase>) override;

    static int Initializer ();
  };

  static int
  X11_ValueTypeAdapter_impl_Initializer =
      ValueTypeAdapter_impl::Initializer ();


  ACE_STATIC_SVC_DECLARE (ValueTypeAdapter_impl)
  ACE_FACTORY_DECLARE (TAOX11_Valuetype, ValueTypeAdapter_impl)

} // namespace TAOX11_NAMESPACEs

#endif /* TAOX11_VALUETYPE_ADAPTER_IMPL_INCLUDED */
