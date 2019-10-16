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

  typedef TAOX11_IDL::traits<CORBA::TypeCode>::ref_type TypeCode_ref;

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<bool>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef bool return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint8_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef uint8_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<char>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef char return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<wchar_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef wchar_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<int16_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef int16_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint16_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef uint16_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<int32_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef int32_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint32_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef uint32_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<int64_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef int64_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<uint64_t>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef uint64_t return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<float>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef float return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<double>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef double return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<long double>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef long double return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<std::string>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef std::string return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<std::wstring>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef std::wstring return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::Any>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::Any return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<IDL::traits<CORBA::Object>::ref_type>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef IDL::traits<CORBA::Object>::ref_type return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<TypeCode_ref>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef TypeCode_ref return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::BooleanSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::BooleanSeq  return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::OctetSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value ;
    typedef CORBA::OctetSeq  return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::CharSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::CharSeq  return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::WCharSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::WCharSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::ShortSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::ShortSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::UShortSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::UShortSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::LongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::LongSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::ULongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
   typedef CORBA::ULongSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::LongLongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::LongLongSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::ULongLongSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::ULongLongSeq  return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::FloatSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::FloatSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::DoubleSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::DoubleSeq return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<CORBA::LongDoubleSeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::LongDoubleSeq  return_type;
  };

  template<>
  struct TAOX11_DynamicAny_Export BasicTypeTraits<TAOX11_NAMESPACE::CORBA::AnySeq>
  {
    static TypeCode_ref const tc_value;
    static CORBA::TCKind const tckind_value;
    typedef CORBA::AnySeq return_type;
  };
}

#endif /* TAOX11_BASIC_TYPE_TRAITS_H */
