/**
 * @file    basic_type_traits.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   Assign values to specialization members in basic_type_traits_t.h.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/dynamic_any/basic_type_traits.h"
#include "tao/x11/object.h"
#include "tao/x11/BooleanSeqC.h"
#include "tao/x11/CharSeqC.h"
#include "tao/x11/OctetSeqC.h"
#include "tao/x11/WCharSeqC.h"
#include "tao/x11/ShortSeqC.h"
#include "tao/x11/UShortSeqC.h"
#include "tao/x11/LongSeqC.h"
#include "tao/x11/ULongSeqC.h"
#include "tao/x11/LongLongSeqC.h"
#include "tao/x11/ULongLongSeqC.h"
#include "tao/x11/FloatSeqC.h"
#include "tao/x11/DoubleSeqC.h"
#include "tao/x11/LongDoubleSeqC.h"
#include "tao/x11/anytypecode/AnySeqC.h"

namespace TAOX11_NAMESPACE
{

  TypeCode_ref const
  BasicTypeTraits<bool>::tc_value= CORBA::_tc_boolean;

  CORBA::TCKind const
  BasicTypeTraits<bool>::tckind_value = CORBA::TCKind::tk_boolean;

  //================================================================

  TypeCode_ref const
  BasicTypeTraits<uint8_t>::tc_value = CORBA::_tc_octet;

  CORBA::TCKind const
  BasicTypeTraits<uint8_t>::tckind_value = CORBA::TCKind::tk_octet;

   //================================================================

  TypeCode_ref const
  BasicTypeTraits<char>::tc_value = CORBA::_tc_char;

  CORBA::TCKind const
  BasicTypeTraits<char>::tckind_value = CORBA::TCKind::tk_char;

  //================================================================

  TypeCode_ref const
  BasicTypeTraits<wchar_t>::tc_value = CORBA::_tc_wchar;

  CORBA::TCKind const
  BasicTypeTraits<wchar_t>::tckind_value = CORBA::TCKind::tk_wchar;

  //================================================================

  TypeCode_ref const
  BasicTypeTraits<int16_t>::tc_value = CORBA::_tc_short;

  CORBA::TCKind const
  BasicTypeTraits<int16_t>::tckind_value = CORBA::TCKind::tk_short;

  //================================================================

  TypeCode_ref const
  BasicTypeTraits<uint16_t>::tc_value = CORBA::_tc_ushort;

  CORBA::TCKind const
  BasicTypeTraits<uint16_t>::tckind_value = CORBA::TCKind::tk_ushort;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<int32_t>::tc_value = CORBA::_tc_long;

  CORBA::TCKind const
  BasicTypeTraits<int32_t>::tckind_value = CORBA::TCKind::tk_long;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<uint32_t>::tc_value = CORBA::_tc_ulong;

  CORBA::TCKind const
  BasicTypeTraits<uint32_t>::tckind_value = CORBA::TCKind::tk_ulong;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<int64_t>::tc_value = CORBA::_tc_longlong;

  CORBA::TCKind const
  BasicTypeTraits<int64_t>::tckind_value = CORBA::TCKind::tk_longlong;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<uint64_t>::tc_value = CORBA::_tc_ulonglong;

  CORBA::TCKind const
  BasicTypeTraits<uint64_t>::tckind_value = CORBA::TCKind::tk_ulonglong;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<float>::tc_value = CORBA::_tc_float;

  CORBA::TCKind const
  BasicTypeTraits<float>::tckind_value = CORBA::TCKind::tk_float;



  //================================================================

  TypeCode_ref const
  BasicTypeTraits<double>::tc_value = CORBA::_tc_double;

  CORBA::TCKind const
  BasicTypeTraits<double>::tckind_value = CORBA::TCKind::tk_double;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<long double>::tc_value = CORBA::_tc_longdouble;

  CORBA::TCKind const
  BasicTypeTraits<long double>::tckind_value = CORBA::TCKind::tk_longdouble;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<std::string>::tc_value = CORBA::_tc_string;

  CORBA::TCKind const
  BasicTypeTraits<std::string>::tckind_value = CORBA::TCKind::tk_string;

  //================================================================

  TypeCode_ref const
  BasicTypeTraits<std::wstring>::tc_value = CORBA::_tc_wstring;

  CORBA::TCKind const
  BasicTypeTraits<std::wstring>::tckind_value = CORBA::TCKind::tk_wstring;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::Any>::tc_value = CORBA::_tc_any;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Any>::tckind_value = CORBA::TCKind::tk_any;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::object_reference< CORBA::Object> >::tc_value = CORBA::_tc_Object;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::object_reference< CORBA::Object>>::tckind_value = CORBA::TCKind::tk_objref;

  //================================================================

  TypeCode_ref const
  BasicTypeTraits<TypeCode_ref>::tc_value = CORBA::_tc_TypeCode;

  CORBA::TCKind const
  BasicTypeTraits<TypeCode_ref>::tckind_value = CORBA::TCKind::tk_TypeCode;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::BooleanSeq>::tc_value = CORBA::_tc_BooleanSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::BooleanSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::OctetSeq>::tc_value = CORBA::_tc_OctetSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::OctetSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::CharSeq>::tc_value = CORBA::_tc_CharSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::CharSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::WCharSeq>::tc_value = CORBA::_tc_WCharSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::WCharSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::ShortSeq>::tc_value = CORBA::_tc_ShortSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ShortSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::UShortSeq>::tc_value = CORBA::_tc_UShortSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::UShortSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::LongSeq>::tc_value = CORBA::_tc_LongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::ULongSeq>::tc_value = CORBA::_tc_ULongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ULongSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::LongLongSeq>::tc_value = CORBA::_tc_LongLongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongLongSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::ULongLongSeq>::tc_value = CORBA::_tc_ULongLongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ULongLongSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::FloatSeq>::tc_value = CORBA::_tc_FloatSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::FloatSeq>::tckind_value = CORBA::TCKind::tk_sequence;



  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::DoubleSeq>::tc_value = CORBA::_tc_DoubleSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::DoubleSeq>::tckind_value = CORBA::TCKind::tk_sequence;



  //================================================================

  TypeCode_ref const
  BasicTypeTraits<CORBA::LongDoubleSeq>::tc_value = CORBA::_tc_LongDoubleSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongDoubleSeq>::tckind_value = CORBA::TCKind::tk_sequence;


  //================================================================

  TypeCode_ref const
  BasicTypeTraits<TAOX11_NAMESPACE::CORBA::AnySeq>::tc_value = CORBA::_tc_AnySeq;

  CORBA::TCKind const
  BasicTypeTraits<TAOX11_NAMESPACE::CORBA::AnySeq>::tckind_value = CORBA::TCKind::tk_sequence;


}



