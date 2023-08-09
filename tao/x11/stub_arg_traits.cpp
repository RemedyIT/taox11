/**
 * @file    stub_arg_traits.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 argument traits for client stubs
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/stub_arg_traits.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/system_exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBA::Boolean operator<< (TAO_OutputCDR & strm,
    TAOX11_CORBA::object_traits<TAOX11_CORBA::Object>::ref_type _tao_objref)
{
  if (_tao_objref && (_tao_objref->get_proxy () == nullptr))
  {
    // No TAO proxy (probably LocalObject) so nothing to marshal; throw exception
    throw TAO_CORBA::MARSHAL (4, TAO_CORBA::COMPLETED_NO);
  }
  const TAO_CORBA::Object* _tao_corba_obj = _tao_objref ? _tao_objref->get_proxy ()->get () : nullptr;
  return (strm << _tao_corba_obj);
}

TAO_CORBA::Boolean operator>> (TAO_InputCDR & strm,
    TAOX11_CORBA::object_traits<TAOX11_CORBA::Object>::ref_type& _tao_objref)
{
  TAO_CORBA::Object_ptr _obj_ptr {};

  if (!(strm >> _obj_ptr))
  {
    return false;
  }

  _tao_objref = TAOX11_CORBA::Object::_narrow (TAOX11_NAMESPACE::Object_proxy (_obj_ptr));

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
