/**
 * @file    any_basic_impl_t.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any_impl class for basic data
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_BASIC_IMPL_T_H
#define TAOX11_ANY_BASIC_IMPL_T_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/marshal_policy_t.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class Any_Basic_Impl_T
   *
   * @brief Template Any class for basic types.
   *
   * Used for all basic IDL types except bounded (w)strings.
   */
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  class Any_Basic_Impl_T : public Any_Impl
  {
  public:
    using impl_type = Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>;
    using ref_type = std::shared_ptr<impl_type>;
    using value_type = typename TRAITS::value_type;

    Any_Basic_Impl_T (CORBA::typecode_reference,
                      value_type val);
    ~Any_Basic_Impl_T () override = default;

    static void insert (CORBA::Any &,
                        CORBA::typecode_reference,
                        value_type);
    static bool extract (const CORBA::Any &,
                         CORBA::typecode_reference,
                         value_type &);

    bool marshal_value (TAO_OutputCDR &) override;
    inline bool demarshal_value (TAO_InputCDR &cdr)
    {
      return MARSHAL_POLICY<TRAITS>::demarshal_value (cdr, this->value_);
    }

    void _tao_decode (TAO_InputCDR &) override;

    static Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> *create_empty (CORBA::typecode_reference);

  protected:
    value_type value_;
  };

  /**
   * @class Any_Object_Impl_T
   *
   * @brief Template Any class for object references.
   */
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  class Any_Object_Impl_T : public Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>
  {
  public:
    using impl_type = Any_Object_Impl_T<TRAITS, MARSHAL_POLICY>;
    using ref_type = std::shared_ptr<impl_type>;
    using value_type = typename TRAITS::value_type;

    Any_Object_Impl_T (CORBA::typecode_reference,
                       value_type val);
    ~Any_Object_Impl_T () override = default;

    static void insert (CORBA::Any &,
                        CORBA::typecode_reference,
                        value_type);
    static bool extract (const CORBA::Any &,
                         CORBA::typecode_reference,
                         value_type &);

    bool to_object (IDL::traits<CORBA::Object>::ref_type&) const override;

    static Any_Object_Impl_T<TRAITS, MARSHAL_POLICY> *create_empty (CORBA::typecode_reference);
  };

  /**
   * @class Any_Value_Impl_T
   *
   * @brief Template Any class for valuetype references.
   */
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  class Any_Value_Impl_T : public Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>
  {
  public:
    using impl_type = Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>;
    using ref_type = std::shared_ptr<impl_type>;
    using value_type = typename TRAITS::value_type;

    Any_Value_Impl_T (CORBA::typecode_reference,
                      value_type val);
    ~Any_Value_Impl_T () override = default;

    bool marshal_type (TAO_OutputCDR &) override;

    static void insert (CORBA::Any &,
                        CORBA::typecode_reference,
                        value_type);
    static bool extract (const CORBA::Any &,
                         CORBA::typecode_reference,
                         value_type &);

    bool to_value (CORBA::valuetype_reference<CORBA::ValueBase>&) const override;

    static Any_Value_Impl_T<TRAITS, MARSHAL_POLICY> *create_empty (CORBA::typecode_reference);
  };

  /**
   * @class Any_AbstractBase_Impl_T
   *
   * @brief Template Any class for abstract interface references.
   */
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  class Any_AbstractBase_Impl_T : public Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>
  {
  public:
    using impl_type = Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY>;
    using ref_type = std::shared_ptr<impl_type>;
    using value_type = typename TRAITS::value_type;

    Any_AbstractBase_Impl_T (CORBA::typecode_reference, value_type val);
    ~Any_AbstractBase_Impl_T () override = default;

    static void insert (CORBA::Any &,
                        CORBA::typecode_reference,
                        value_type);
    static bool extract (const CORBA::Any &,
                         CORBA::typecode_reference,
                         value_type &);

    bool to_abstract_base (CORBA::abstractbase_reference<CORBA::AbstractBase>&) const override;

    static Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY> *create_empty (CORBA::typecode_reference);
  };
}

#include "tao/x11/anytypecode/any_basic_impl_t.cpp"

#endif /* TAOX11_ANY_BASIC_IMPL_T_H */
