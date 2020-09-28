/**
 * @file    basic_type_traits.h
 * @author  Marijke Henstmengel
 *
 * @brief   Specializations of template traits in basic_type_traits_t.h.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_BASIC_TYPE_TRAITS_H
#define TAOX11_BASIC_TYPE_TRAITS_H

#pragma once

#include "tao/x11/dynamic_any/basic_type_traits_t.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/AnySeqC.h"
#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"

namespace TAOX11_NAMESPACE
{

  using TypeCode_ref = TAOX11_IDL::traits<CORBA::TypeCode>::ref_type;

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<bool>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = bool;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint8_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = uint8_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<char>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = char;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<wchar_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = wchar_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<int16_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = int16_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint16_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = uint16_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<int32_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = int32_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint32_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = uint32_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<int64_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = int64_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint64_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = uint64_t;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<float>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = float;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<double>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = double;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<long double>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = long double;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<std::string>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = std::string;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<std::wstring>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = std::wstring;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::Any>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::Any;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<IDL::traits<CORBA::Object>::ref_type>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = IDL::traits<CORBA::Object>::ref_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<TypeCode_ref>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = TypeCode_ref;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::BooleanSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::BooleanSeq ;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::OctetSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value ;
    using return_type = CORBA::OctetSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::CharSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::CharSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::WCharSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::WCharSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::ShortSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::ShortSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::UShortSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::UShortSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::LongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::LongSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::ULongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::ULongSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::LongLongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::LongLongSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::ULongLongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::ULongLongSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::FloatSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::FloatSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::DoubleSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::DoubleSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::LongDoubleSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::LongDoubleSeq;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<TAOX11_NAMESPACE::CORBA::AnySeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    using return_type = CORBA::AnySeq;
  };
}

#endif /* TAOX11_BASIC_TYPE_TRAITS_H */
