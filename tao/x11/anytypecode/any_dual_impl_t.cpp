/**
 * @file    any_dual_impl_t.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Any_impl class for data with copying
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_ANY_DUAL_IMPL_T_CPP
#define TAOX11_ANY_DUAL_IMPL_T_CPP

#include "tao/x11/anytypecode/any_dual_impl_t.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_Memory.h"

namespace TAOX11_NAMESPACE
{
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::Any_Dual_Impl_T (
      CORBA::typecode_reference tc,
      const value_type &val)
    : Any_Impl (std::move(tc)),
      value_ (val)
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY> void
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::value (
      const value_type & val)
  {
    this->value_ = val;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::Any_Dual_Impl_T (
      CORBA::typecode_reference tc,
      value_type && val)
    : Any_Impl (std::move(tc)),
      value_ (std::move (val))
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::Any_Dual_Impl_T (
      CORBA::typecode_reference tc)
    : Any_Impl (std::move(tc))
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::insert (
      CORBA::Any & any,
      CORBA::typecode_reference tc,
      value_type && value)
  {
    impl_type *new_impl {};
    ACE_NEW (new_impl,
             impl_type (std::move(tc), std::move (value)));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::insert_copy (
      CORBA::Any & any,
      CORBA::typecode_reference tc,
      const value_type & value)
  {
    impl_type *new_impl {};
    ACE_NEW (new_impl,
             impl_type (std::move(tc), value));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::extract (
      const CORBA::Any & any,
      CORBA::typecode_reference tc,
      value_type & _tao_elem)
  {
    try
      {
        Any_Impl::ref_type const impl = any.impl ();

        if (!impl)
          {
            return false;
          }

        CORBA::typecode_reference any_tc = impl->type ();
        bool const _tao_equiv = any_tc->equivalent (tc);

        if (!_tao_equiv)
          {
            return false;
          }

        if (impl && !impl->encoded ())
          {
            ref_type const narrow_impl =
              std::dynamic_pointer_cast <impl_type> (impl);

            if (!narrow_impl)
              {
                return false;
              }

            _tao_elem = narrow_impl->value_;
            return true;
          }

        impl_type *replacement =
            new impl_type (any_tc);

        ref_type replacement_safety (replacement);

        // We know this will work since the unencoded case is covered above.
        Unknown_IDL_Type::ref_type const unk =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!unk)
          return false;

        // We don't want the rd_ptr of unk to move, in case it is
        // shared by another Any. This copies the state, not the buffer.
        TAO_InputCDR for_reading (unk->_tao_get_cdr ());

        bool const good_decode =
            replacement_safety->demarshal_value (for_reading);

        if (good_decode)
          {
            _tao_elem = replacement_safety->value_;
            const_cast<CORBA::Any &> (any).replace (replacement_safety);
            return true;
          }
      }
    catch (const CORBA::Exception&)
      {
      }

    return false;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::marshal_value (
      TAO_OutputCDR &cdr)
  {
    return MARSHAL_POLICY<TRAITS>::marshal_value (cdr, this->value_);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Dual_Impl_T<TRAITS, MARSHAL_POLICY>::_tao_decode (
      TAO_InputCDR &cdr)
  {
    if (! this->demarshal_value (cdr))
      {
        throw CORBA::MARSHAL ();
      }
  }

} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_ANY_DUAL_IMPL_T_CPP */
