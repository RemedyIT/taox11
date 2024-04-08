/**
 * @file    any_basic_impl_t.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any_impl class for basic data
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_BASIC_IMPL_T_CPP
#define TAOX11_ANY_BASIC_IMPL_T_CPP

#include "tao/x11/anytypecode/any_basic_impl_t.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/CDR.h"
#include "tao/x11/log.h"

#include "ace/Auto_Ptr.h"

namespace TAOX11_NAMESPACE
{
  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::Any_Basic_Impl_T (CORBA::typecode_reference tc,
                                                              value_type val)
    : Any_Impl (std::move(tc)),
      value_ (std::move(val))
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::insert (CORBA::Any & any,
                                    CORBA::typecode_reference tc,
                                    value_type value)
  {
    Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> *new_impl {};
    ACE_NEW (new_impl,
             Any_Basic_Impl_T (std::move(tc),
                               std::move(value)));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::extract (const CORBA::Any & any,
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
          std::dynamic_pointer_cast<impl_type> (impl);

        if (!narrow_impl)
          {
            return false;
          }

        _tao_elem = narrow_impl->value_;
        return true;
      }

      Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> * const replacement =
        Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::create_empty (any_tc);

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
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::marshal_value (TAO_OutputCDR &cdr)
  {
    return MARSHAL_POLICY<TRAITS>::marshal_value (cdr, this->value_);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> *
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::create_empty (CORBA::typecode_reference tc)
  {
    impl_type * retval = new (std::nothrow) impl_type (std::move(tc), value_type (0));
    if (!retval)
      throw TAO_CORBA::NO_MEMORY ();
    return retval;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY>::_tao_decode (TAO_InputCDR &cdr)
  {
    if (! this->demarshal_value (cdr))
      {
        throw CORBA::MARSHAL ();
      }
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Object_Impl_T<TRAITS, MARSHAL_POLICY>::Any_Object_Impl_T (CORBA::typecode_reference tc,
                                                                value_type val)
   : Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> (std::move(tc), std::move(val))
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Object_Impl_T<TRAITS, MARSHAL_POLICY>::to_object (IDL::traits<CORBA::Object>::ref_type &_tao_elem) const
  {
    _tao_elem = this->value_;
    return true;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Object_Impl_T<TRAITS, MARSHAL_POLICY>::insert (CORBA::Any & any,
                                    CORBA::typecode_reference tc,
                                    value_type value)
  {
    impl_type *new_impl {};
    ACE_NEW (new_impl,
             impl_type (std::move(tc),
                        std::move(value)));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Object_Impl_T<TRAITS, MARSHAL_POLICY>::extract (const CORBA::Any & any,
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
              std::dynamic_pointer_cast<impl_type> (impl);

        if (!narrow_impl)
          {
            return false;
          }

        _tao_elem = narrow_impl->value_;
        return true;
      }

      impl_type * const replacement =
          impl_type::create_empty (any_tc);

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
  Any_Object_Impl_T<TRAITS, MARSHAL_POLICY> *
  Any_Object_Impl_T<TRAITS, MARSHAL_POLICY>::create_empty (CORBA::typecode_reference tc)
  {
    impl_type * retval =  new (std::nothrow) impl_type (std::move(tc), value_type (0));
    if (!retval)
      throw TAO_CORBA::NO_MEMORY ();
    return retval;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>::Any_Value_Impl_T (CORBA::typecode_reference tc,
                                                              value_type val)
   : Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> (tc, val)
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>::marshal_type (TAO_OutputCDR &cdr)
  {
    return (cdr << this->value_->_obv_typecode ());
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>::to_value (CORBA::valuetype_reference<CORBA::ValueBase> &_tao_elem) const
  {
    _tao_elem = this->value_;
    return true;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>::insert (
      CORBA::Any & any,
      CORBA::typecode_reference tc,
      value_type value)
  {
    impl_type *new_impl {};
    ACE_NEW (new_impl,
             impl_type (std::move(tc),
                        std::move(value)));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>::extract (
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
              std::dynamic_pointer_cast<impl_type> (impl);

        if (!narrow_impl)
          {
            return false;
          }

        _tao_elem = narrow_impl->value_;
        return true;
      }

      impl_type * const replacement =
          impl_type::create_empty (any_tc);

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
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY> *
  Any_Value_Impl_T<TRAITS, MARSHAL_POLICY>::create_empty (CORBA::typecode_reference tc)
  {
    impl_type * retval = new (std::nothrow) impl_type (std::move(tc), value_type (0));
    if (!retval)
      throw TAO_CORBA::NO_MEMORY ();
    return retval;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY>::Any_AbstractBase_Impl_T (CORBA::typecode_reference tc,
                                                                value_type val)
   : Any_Basic_Impl_T<TRAITS, MARSHAL_POLICY> (std::move(tc), std::move(val))
  {
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY>::to_abstract_base (CORBA::abstractbase_reference<CORBA::AbstractBase> &_tao_elem) const
  {
    _tao_elem = this->value_;
    return true;
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  void
  Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY>::insert (
      CORBA::Any & any,
      CORBA::typecode_reference tc,
      value_type value)
  {
    impl_type *new_impl {};
    ACE_NEW (new_impl,
             impl_type (std::move(tc),
                        std::move(value)));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  template<typename TRAITS, template <typename T> class MARSHAL_POLICY>
  bool
  Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY>::extract (
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
              std::dynamic_pointer_cast<impl_type> (impl);

        if (!narrow_impl)
          {
            return false;
          }

        _tao_elem = narrow_impl->value_;
        return true;
      }

      impl_type * const replacement =
          impl_type::create_empty (any_tc);

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
  Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY> *
  Any_AbstractBase_Impl_T<TRAITS, MARSHAL_POLICY>::create_empty (CORBA::typecode_reference tc)
  {
    impl_type * retval = new (std::nothrow) impl_type (std::move(tc), value_type (0));
    if (!retval)
      throw TAO_CORBA::NO_MEMORY ();
    return retval;
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_ANY_BASIC_IMPL_T_CPP */
