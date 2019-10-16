/**
 * @file    valuetype_adapter_impl.cpp
 * @author  Martin Corino
 *
 * @brief   Concrete valuetype adapter impl
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/valuetype/valuetype_adapter_impl.h"
#include "tao/x11/valuetype/valuetype_proxies.h"
#include "tao/x11/valuetype/abstractbase_proxy.h"

namespace TAOX11_NAMESPACE
{
  bool
  ValueTypeAdapter_impl::stream_to_value (
      TAO_InputCDR &cdr,
      CORBA::valuetype_reference<CORBA::ValueBase> &vbref)
  {
    return (cdr >> vbref);
  }

  bool
  ValueTypeAdapter_impl::stream_to_abstract_base (
      TAO_InputCDR &cdr,
      CORBA::abstractbase_reference<CORBA::AbstractBase> &absref)
  {
    return (cdr >> absref);
  }

  bool
  ValueTypeAdapter_impl::value_to_stream (
      TAO_OutputCDR &cdr,
      CORBA::valuetype_reference<CORBA::ValueBase> vbref)
  {
    return (cdr << vbref);
  }

  bool
  ValueTypeAdapter_impl::abstract_base_to_stream (
      TAO_OutputCDR &cdr,
      CORBA::abstractbase_reference<CORBA::AbstractBase> absref)
  {
    return (cdr << absref);
  }

  int ValueTypeAdapter_impl::Initializer ()
  {
    ValueTypeAdapter::set_concrete_adapter_name ("Concrete_ValueTypeAdapter");

    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_ValueTypeAdapter_impl
        );
  }

  ACE_STATIC_SVC_DEFINE (
      ValueTypeAdapter_impl,
      ACE_TEXT ("Concrete_ValueTypeAdapter"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (ValueTypeAdapter_impl),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAOX11_Valuetype, ValueTypeAdapter_impl)

} // namespace TAOX11_NAMESPACE
