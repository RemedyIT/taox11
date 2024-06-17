/**
 * @file    any_dual_impl_t.h
 * @author  Marijke Hengstmengel
 *
 * @brief   TAOX11 CORBA Any_impl class for data with copying
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_DUAL_IMPL_T_H
#define TAOX11_ANY_DUAL_IMPL_T_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/marshal_policy_t.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class Any_Dual_Impl_T
   *
   * @brief Template Any class for IDL types with 2 modes of insertion
   *
   * Used for the IDL types that have copying and non-copying insertion,
   * but which are not (de)marshaled as pointers - Any, struct, union,
   * array and exception.
   */
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  class Any_Dual_Impl_T : public Any_Impl
  {
  public:
    using impl_type = Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>;
    using ref_type = std::shared_ptr<impl_type>;
    using value_type = typename TRAITS::value_type;

    Any_Dual_Impl_T (CORBA::typecode_reference,
                     value_type&&);
    Any_Dual_Impl_T (CORBA::typecode_reference,
                     const value_type &);
    Any_Dual_Impl_T (CORBA::typecode_reference);
    ~Any_Dual_Impl_T () override = default;

    static void insert (CORBA::Any &,
                        CORBA::typecode_reference,
                        value_type&&);
    static void insert_copy (CORBA::Any &,
                             CORBA::typecode_reference,
                             const value_type &);
    static bool extract (const CORBA::Any &,
                         CORBA::typecode_reference,
                         value_type&);

    bool marshal_value (TAO_OutputCDR &) override;
    bool demarshal_value (TAO_InputCDR &cdr)
    {
      return MARSHAL_POLICY<TRAITS>::demarshal_value (cdr, this->value_);
    }

    void _tao_decode (TAO_InputCDR &) override;

  protected:
    void value (const value_type &);

    value_type value_;
  };
} // namespace TAOX11_NAMESPACE

#include "tao/x11/anytypecode/any_dual_impl_t.cpp"

#endif /* TAOX11_ANY_DUAL_IMPL_T_H */
