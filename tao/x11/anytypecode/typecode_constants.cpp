/**
 * @file    typecode_constants.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA standard typecodes
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/SystemExceptionA.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/typecode_constants.h"
// to get declaration of __tao::TAOX11_NAMESPACE::CORBA::_tc_TypeCode
#include "tao/x11/Typecode_typesCP.h"

namespace __tao
{
  namespace TAOX11_NAMESPACE
  {
    namespace CORBA
    {
      TAO_CORBA::TypeCode_ptr const _tc_TypeCode = TAO_CORBA::_tc_TypeCode; // dummy to satify linker
    }
  }
}

namespace TAOX11_NAMESPACE
{

  namespace CORBA
  {
    /**
     * @name TypeCode Constants
     *
     * All the TypeCode constants
     */
    //@{
    CORBA::typecode_reference const _tc_null = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_null, true);
    CORBA::typecode_reference const _tc_void = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_void, true);
    CORBA::typecode_reference const _tc_boolean = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_boolean, true);
    CORBA::typecode_reference const _tc_char = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_char, true);
    CORBA::typecode_reference const _tc_wchar = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_wchar, true);
    CORBA::typecode_reference const _tc_short = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_short, true);
    CORBA::typecode_reference const _tc_ushort = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_ushort, true);
    CORBA::typecode_reference const _tc_long = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_long, true);
    CORBA::typecode_reference const _tc_ulong = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_ulong, true);
    CORBA::typecode_reference const _tc_longlong = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_longlong, true);
    CORBA::typecode_reference const _tc_ulonglong = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_ulonglong, true);
    CORBA::typecode_reference const _tc_float = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_float, true);
    CORBA::typecode_reference const _tc_double = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_double, true);
    CORBA::typecode_reference const _tc_longdouble = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_longdouble, true);
    CORBA::typecode_reference const _tc_octet = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_octet, true);
    CORBA::typecode_reference const _tc_any = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_any, true);
    CORBA::typecode_reference const _tc_TypeCode = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_TypeCode, true);
    CORBA::typecode_reference const _tc_Principal = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_Principal, true);

    CORBA::typecode_reference const _tc_string = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_string, true);
    CORBA::typecode_reference const _tc_wstring = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_wstring, true);

    CORBA::typecode_reference const _tc_Object = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_Object, true);
    CORBA::typecode_reference const _tc_Component = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_Component, true);
    CORBA::typecode_reference const _tc_Home = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_Home, true);
    //@}

#define TAO_SYSTEM_EXCEPTION(name) \
    CORBA::typecode_reference const _tc_ ## name = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_ ## name, true)
    TAO_SYSTEM_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
