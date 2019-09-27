/**
 * @file    valuetype_adapter.h
 * @author  Martin Corino
 *
 * @brief   Valuetype adapter interface
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_VALUETYPE_ADAPTER_INCLUDED
#define TAOX11_VALUETYPE_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/stddef.h"
#include "tao/x11/valuetype/abstractbase_traits_t.h"
#include "tao/x11/valuetype/valuetype_traits_t.h"

#if !defined(CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  class ORB_Proxy;

  class TAOX11_Export ValueTypeAdapter
    : public ACE_Service_Object
  {
  public:
    ~ValueTypeAdapter() = default;

    virtual bool
    stream_to_value (
        TAO_InputCDR &,
        CORBA::valuetype_reference<CORBA::ValueBase>&) = 0;

    virtual bool
    stream_to_abstract_base (
        TAO_InputCDR &,
        CORBA::abstractbase_reference<CORBA::AbstractBase> &) = 0;

    virtual bool
    value_to_stream (
        TAO_OutputCDR &,
        CORBA::valuetype_reference<CORBA::ValueBase>) = 0;

    virtual bool
    abstract_base_to_stream (
        TAO_OutputCDR &,
        CORBA::abstractbase_reference<CORBA::AbstractBase>) = 0;

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

    static ValueTypeAdapter *value_type_adapter ();

  private:
    static std::string concrete_adapter_name_;
  };
} // namespace TAOX11_NAMESPACE

#endif

#endif /* TAOX11_VALUETYPE_ADAPTER_INCLUDED */
