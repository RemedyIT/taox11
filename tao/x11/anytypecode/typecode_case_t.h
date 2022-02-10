/**
 * @file    typecode_case_t.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_TYPECODE_CASE_T_H
#define TAOX11_TYPECODE_CASE_T_H

#pragma once

#include "tao/x11/tao_corba.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Case_Base_T.h"

namespace TAOX11_NAMESPACE
{
  namespace TypeCode
  {
    /**
     * @class Case_T
     *
     * @brief Representation of an OMG IDL defined @c union @c case.
     *
     * A @c Case_T contains the corresponding case label value, name and
     * pointer to the @c CORBA::TypeCode for a given OMG IDL @c union
     * @c case.  For
     * example, the cases in following OMG IDL @c union:
     *
     * \code
     *   union Foo switch (short)
     *   {
     *     case 0:
     *     case 1:  short a;
     *     case 2:  long  b;
     *     default: octet c;
     *   };
     * \endcode
     *
     * would be represented using the following statically instantiated
     * @c TAO::TypeCode::Case_T array:
     *
     * \code
     *   using Foo_Case = TAO::TypeCode::Case_T<CORBA::Short, char const *>;
     *   Foo_Case _tao_cases_Foo[] =
     *     {
     *       Foo_Case (0, "a", std::addressof(CORBA::_tc_short)),
     *       Foo_Case (1, "a", std::addressof(CORBA::_tc_short)),
     *       Foo_Case (2, "b", std::addressof(CORBA::_tc_long)),
     *       Foo_Case (0, "c", std::addressof(CORBA::_tc_octet))
     *     };
     * \endcode
     *
     * The @c default index is passed directly to the
     * @c TAO::TypeCode::Union constructor.
     *
     * The template parameter @a DISCRIMINATOR_TYPE is the
     * corresponding C++ type for the IDL defined @c union
     * discriminator type.  For example, a @c union with an IDL @c
     * short discriminator will have a corresponding @c CORBA::Short
     * C++ type.  This template parameter is necessary to allow the
     * actual @c case label value to be stored as its actual type,
     * which is particularly important when marshaling that value into
     * a CDR stream.
     *
     * The template parameter @a StringType is either @c char @c
     * const @c * or @c CORBA::String_var.  The latter is only used
     * when creating @c CORBA::tk_union @c TypeCode dynamically, such
     * as through the TypeCodeFactory.
     */
    template <typename DiscriminatorType,
              typename StringType,
              typename TypeCodeType>
    class Case_T : public TAO_TAO::TypeCode::Case<StringType, TypeCodeType>
    {
    public:

      /// Constructor.
      Case_T (DiscriminatorType member_label,
              char const * member_name,
              TypeCodeType member_type)
        : TAO_TAO::TypeCode::Case<
              StringType,
              TypeCodeType> (member_name, member_type)
        , label_ (member_label)
      {
      }

      /// Constructor.
      /**
       * Constructor only used in the dynamically constructed @c union
       * @c TypeCode case.
       */
      Case_T (DiscriminatorType member_label)
        : TAO_TAO::TypeCode::Case<StringType, TypeCodeType> ()
        , label_ (member_label)
      {
      }
      virtual ~Case_T () = default;


      /**
       * @name @c TAO::TypeCode::Case Methods
       *
       * Methods required by the @c TAO::TypeCode::Case abstract base
       * class.
       *
       * @see @c TAO::TypeCode::Case
       */
      //@{
      TAO_TAO::TypeCode::Case<StringType, TypeCodeType> * clone () const override;
      TAO_CORBA::Any* label () const override;
      bool marshal_label (TAO_OutputCDR & cdr) const override;
      bool equal_label (uint32_t index, TAO_CORBA::TypeCode_ptr tc) const override;
      //@}

    private:
      /// IDL @c union case/member label value.
      DiscriminatorType const label_;
    };
  } // namespace TypeCode
} // namespace TAOX11_NAMESPACE

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/x11/anytypecode/typecode_case_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("typecode_case_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAOX11_TYPECODE_CASE_T_H */
