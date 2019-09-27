/**
 * @file    typecode_constants.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA standard typecodes
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_TYPECODE_CONSTANTS_H
#define TAOX11_TYPECODE_CONSTANTS_H

#include "tao/x11/Typecode_typesC.h"
#include "tao/x11/anytypecode/typecode_ref.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"

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
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_null;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_void;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_boolean;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_char;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_wchar;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_short;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_ushort;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_long;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_ulong;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_longlong;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_ulonglong;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_float;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_double;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_longdouble;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_octet;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_any;
    // from Typecode_typesC.h
    // extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_TypeCode;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_Principal;

    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_string;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_wstring;

    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_Object;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_Component;
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_Home;
    //@}

  #define TAO_SYSTEM_EXCEPTION_LIST \
    TAO_SYSTEM_EXCEPTION(UNKNOWN); \
    TAO_SYSTEM_EXCEPTION(BAD_PARAM); \
    TAO_SYSTEM_EXCEPTION(NO_MEMORY); \
    TAO_SYSTEM_EXCEPTION(IMP_LIMIT); \
    TAO_SYSTEM_EXCEPTION(COMM_FAILURE); \
    TAO_SYSTEM_EXCEPTION(INV_OBJREF); \
    TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST); \
    TAO_SYSTEM_EXCEPTION(NO_PERMISSION); \
    TAO_SYSTEM_EXCEPTION(INTERNAL); \
    TAO_SYSTEM_EXCEPTION(MARSHAL); \
    TAO_SYSTEM_EXCEPTION(INITIALIZE); \
    TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT); \
    TAO_SYSTEM_EXCEPTION(BAD_TYPECODE); \
    TAO_SYSTEM_EXCEPTION(BAD_OPERATION); \
    TAO_SYSTEM_EXCEPTION(NO_RESOURCES); \
    TAO_SYSTEM_EXCEPTION(NO_RESPONSE); \
    TAO_SYSTEM_EXCEPTION(PERSIST_STORE); \
    TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER); \
    TAO_SYSTEM_EXCEPTION(TRANSIENT); \
    TAO_SYSTEM_EXCEPTION(FREE_MEM); \
    TAO_SYSTEM_EXCEPTION(INV_IDENT); \
    TAO_SYSTEM_EXCEPTION(INV_FLAG); \
    TAO_SYSTEM_EXCEPTION(INTF_REPOS); \
    TAO_SYSTEM_EXCEPTION(BAD_CONTEXT); \
    TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER); \
    TAO_SYSTEM_EXCEPTION(DATA_CONVERSION); \
    TAO_SYSTEM_EXCEPTION(INV_POLICY); \
    TAO_SYSTEM_EXCEPTION(REBIND); \
    TAO_SYSTEM_EXCEPTION(TIMEOUT); \
    TAO_SYSTEM_EXCEPTION(TRANSACTION_UNAVAILABLE); \
    TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE); \
    TAO_SYSTEM_EXCEPTION(TRANSACTION_REQUIRED); \
    TAO_SYSTEM_EXCEPTION(TRANSACTION_ROLLEDBACK); \
    TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION); \
    TAO_SYSTEM_EXCEPTION(CODESET_INCOMPATIBLE); \
    TAO_SYSTEM_EXCEPTION(BAD_QOS); \
    TAO_SYSTEM_EXCEPTION(INVALID_ACTIVITY); \
    TAO_SYSTEM_EXCEPTION(ACTIVITY_COMPLETED); \
    TAO_SYSTEM_EXCEPTION(ACTIVITY_REQUIRED); \
    TAO_SYSTEM_EXCEPTION(THREAD_CANCELLED);

    // = Typecode constants for system exceptions.
#define TAO_SYSTEM_EXCEPTION(name) \
    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_ ## name
    TAO_SYSTEM_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION
    //@}

    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_Current;

//    extern TAOX11_AnyTypeCode_Export CORBA::typecode_reference const _tc_NamedValue;
  } // namespace CORBA

} // namespace TAOX11_NAMESPACE

#endif
