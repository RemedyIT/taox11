/**
 * @file    typecode_case_t.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */


#ifndef TAOX11_TYPECODE_CASE_T_CPP
#define TAOX11_TYPECODE_CASE_T_CPP

#include "tao/x11/anytypecode/typecode_case_t.h"

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/CORBA_macros.h"

#include "tao/x11/anytypecode/any_util.h"

namespace TAOX11_NAMESPACE
{
  namespace TypeCode
  {
    template <typename T>
    struct Case_Traits
    {
      inline static T cdr_from (T v)
      {
        return v;
      }
    };

    // Specializations for char type; requires wrapper CDR streaming.

    template <>
    struct Case_Traits<char>
    {
      inline static ACE_OutputCDR::from_char cdr_from (char v)
      {
        return ACE_OutputCDR::from_char (v);
      }
    };
  } // End TypeCode namespace
}  // End TAOX11_NAMESPACE

// ----------------------------------------------------------------


template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
TAO_TAO::TypeCode::Case<StringType, TypeCodeType> *
TAOX11_NAMESPACE::TypeCode::Case_T<DiscriminatorType, StringType, TypeCodeType>::clone () const
{
  using case_type = Case_T<DiscriminatorType,
                 StringType,
                 TypeCodeType>;

  // The compiler generated memberwise copy constructor is sufficient.
  TAO_TAO::TypeCode::Case<StringType, TypeCodeType> * p = new (std::nothrow) case_type (*this);
  if (!p)
    throw TAO_CORBA::NO_MEMORY ();
  return p;
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
bool
TAOX11_NAMESPACE::TypeCode::Case_T<DiscriminatorType, StringType, TypeCodeType>::marshal_label (TAO_OutputCDR & cdr) const
{
  return
    (cdr <<
        TAOX11_NAMESPACE::TypeCode::Case_Traits<DiscriminatorType>::cdr_from (this->label_));
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
bool
TAOX11_NAMESPACE::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::equal_label (uint32_t index,
                                                  TAO_CORBA::TypeCode_ptr tao_tc
                                                  ) const
{
  TAO_CORBA::Any_var tao_any = tao_tc->member_label (index);
  CORBA::Any any;
  Any_Util::tao_any_to_x11_any (tao_any.in (), any);
  // The equality operator == below is guaranteed to be defined for
  // the discriminator type since an IDL union discriminator type must
  // be any of the following: (1) an integer, (2) a character, (3) a
  // boolean, or (4) an enumeration.

  DiscriminatorType tc_label;
  if ((any >>= tc_label)
      && this->label_ == tc_label)
    {
      return true;
    }

  return false;
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
TAO_CORBA::Any*
TAOX11_NAMESPACE::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::label () const
{
  CORBA::Any value;

  value <<= this->label_;

  TAO_CORBA::Any * pval {};

  ACE_NEW_THROW_EX (pval,
                    TAO_CORBA::Any,
                    CORBA::NO_MEMORY ());

  TAO_CORBA::Any_var safe_value (pval);

  Any_Util::x11_any_to_tao_any (value, safe_value.inout ());

  return safe_value._retn ();
}

#endif  /* TAOX11_TYPECODE_CASE_T_CPP */
