/**
 * @file    dynanyutils_t.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 dynanyfactory utils
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_DYNANYUTILS_T_H
#define TAOX11_DYNANYUTILS_T_H

#pragma once

#include "tao/x11/dynamic_any/dynamicany.h"
#include "tao/x11/dynamic_any/basic_type_traits.h"

class TAOX11_DynCommon;

namespace TAOX11_NAMESPACE
{
  // Encapsulates common code for inserting and extracting basic
  // types, parameterized on the basic type.
  template<typename T>
  struct DynAnyBasicTypeUtils
  {
    static void
    insert_value (const T &val,
                  IDL::traits<DynamicAny::TAOX11_DynCommon>::ref_type the_dynany);

    static typename BasicTypeTraits<T>::return_type
    get_value (IDL::traits<DynamicAny::TAOX11_DynCommon>::ref_type the_dynany);
  };

  // Encapsulates code that would otherwise be repeated in
  // DynCommon::set_flag(). Parameterized on the type
  // of dynany impl class that underlies the DynAny arg.
  template<typename T>
  struct DynAnyFlagUtils
  {
    static void
    set_flag_t (IDL::traits<DynamicAny::DynAny>::ref_type component,
                bool destroying);
  };

  // Used by MakeDynAnyUtils below, parameterized on the type of
  // impl class and on {Any | TypeCode | inputCDR}.
  template<typename DA_IMPL, typename ANY_TC>
  struct CreateDynAnyUtils
  {
    static typename IDL::traits<DA_IMPL>::ref_type
     create_dyn_any_t (
        ANY_TC any_tc,
        bool allow_truncation= true);

    static typename IDL::traits<DA_IMPL>::ref_type
      create_dyn_any_t (
        IDL::traits<CORBA::TypeCode>::ref_type tc,
        ANY_TC any_tc,
        bool allow_truncation= true);
  };

  // Code common to DynAnyFactory create_* calls, parameterized on
  // {Any | TypeCode | inputCDR}.
  namespace MakeDynAnyUtils
  {
    template<typename ANY_TC>
    IDL::traits<DynamicAny::DynAny>::ref_type
      make_dyn_any_t (
        IDL::traits<CORBA::TypeCode>::ref_type tc,
        ANY_TC any_tc,
        bool allow_truncation= true);
  }
}//TAOX11_NAMESPACE


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/dynamic_any/dynanyutils_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("tao/x11/dynamic_any/dynanyutils_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAOX11_DYNANYUTILS_T_H */
