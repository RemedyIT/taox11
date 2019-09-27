/**
 * @file    value_base.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueType base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "tao/CDR.h"

#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/typecode_constants.h"
#include "tao/x11/anytypecode/any_basic_impl_t.h"
#include "tao/x11/valuetype/valuetype_proxies.h"
#include "tao/x11/valuetype/value_base.h"
#include "tao/x11/anytypecode/any.h"

namespace TAOX11_NAMESPACE
{

  namespace CORBA
  {

    valuetype_reference<ValueBase> ValueBase::_copy_value () const
    {
      return nullptr;
    }

    bool ValueBase::_obv_marshal (TAO_OutputCDR &strm,
                                  _ref_type vt,
                                  ptrdiff_t formal_type_id)
    {
      return TAO_CORBA::ValueBase::_tao_marshal (
          strm,
          vt ? vt->_get_proxy () : nullptr,
          formal_type_id);
    }

    bool ValueBase::_obv_unmarshal (TAO_InputCDR &strm,
                                    _ref_type& new_vt)
    {
      TAO_CORBA::ValueBase* vptr = nullptr;
      if (TAO_CORBA::ValueBase::_tao_unmarshal (strm, vptr))
      {
        // Release locked value inside proxy (if not nil) and transfer to us.
        new_vt = vptr ? dynamic_cast<Valuetype_proxy_ptr> (vptr)->release_value () : nullptr;
        return true;
      }
      return false;
    }

    CORBA::typecode_reference ValueBase::_obv_typecode () const
    {
      return CORBA::_tc_null;
    }

    ValueBase::ValueBase ()
    {
      this->proxy_ = new Valuetype_proxy (this);
    }
    ValueBase::ValueBase (const ValueBase&)
    {
      this->proxy_ = new Valuetype_proxy (this);
    }
    ValueBase::ValueBase (ValueBase&&)
    {
      this->proxy_ = new Valuetype_proxy (this);
    }

    ValueBase::~ValueBase ()
    {
      this->proxy_->_remove_ref ();
    }

    ValueBase::_shared_ptr_type ValueBase::_lock_shared ()
    {
      ValueBase::_shared_ptr_type vbref = this->self_ref_.lock ();
      if (!vbref)
      {
        vbref = ValueBase::_shared_ptr_type (this, ValueBase::dtor ());
        this->self_ref_ = vbref;
      }
      return vbref;
    }

    void ValueBase::_set_truncatable (bool f)
    {
      this->proxy_->is_truncatable_ = f;
    }

    bool ValueBase::_uses_chunking () const
    {
      return this->proxy_->chunking_;
    }

    bool ValueBase::_is_truncatable () const
    {
      return this->proxy_->is_truncatable_;
    }

    bool ValueBase::_obv_match_formal_type (ptrdiff_t ) const
    {
      // For now: always returns false -> i.e. always marshal type Id
      // except for value boxes (overloaded there).
      return false;
    }

    abstractbase_reference<AbstractBase> ValueBase::_obv_abstract_base ()
    {
      return nullptr;
    }

    CORBA::typecode_reference const _tc_ValueBase = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_ValueBase, true);
    CORBA::typecode_reference const _tc_EventBase = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_EventBase, true);

  } // namespace CORBA

  void
  operator<<= (CORBA::Any& any, TAOX11_CORBA::ValueBase::_ref_type vb)
  {
    Any_Value_Impl_T<IDL::traits< CORBA::ValueBase>,
                     CDR_Marshal_Policy>::insert (any, CORBA::_tc_ValueBase, vb);
  }

  bool
  operator>>= (const CORBA::Any& any, TAOX11_CORBA::ValueBase::_ref_type &vb)
  {
    Any_Impl::ref_type impl = any.impl ();
    return impl->to_value (vb);
  }
} // namespace TAOX11_NAMESPACE
