/**
 * @file    any_arg_traits.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 argument traits for CORBA::Any
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_arg_traits.h"
#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/typecode_constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBA::Boolean operator<< (TAO_OutputCDR & strm,
    const TAOX11_CORBA::Any& _any)
{
  TAOX11_NAMESPACE::Any_Impl::ref_type impl = _any.impl ();

  if (impl)
  {
    return impl->marshal (strm);
  }

  return (strm << TAOX11_CORBA::_tc_null);
}

TAO_CORBA::Boolean operator>> (TAO_InputCDR & strm,
    TAOX11_CORBA::Any& _any)
{
   TAOX11_CORBA::typecode_reference tc;

   if (!(strm >> tc))
   {
     return false;
   }

   try
   {
     TAOX11_NAMESPACE::Unknown_IDL_Type *impl {};
     ACE_NEW_RETURN (impl,
                     TAOX11_NAMESPACE::Unknown_IDL_Type (tc),
                     false);

     TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type safe_impl (impl);
     _any.replace (safe_impl);
     impl->_tao_decode (strm);
   }
   catch (const TAOX11_CORBA::Exception&)
   {
     return false;
   }
   return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
