/**
 * @file    valuetype_proxies.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueType TAO proxy classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/valuetype/valuetype_proxies.h"

namespace TAOX11_NAMESPACE
{

  TAO_CORBA::ValueBase* Valuetype_proxy::_copy_value ()
  {
    return nullptr; // noop
  }

  void Valuetype_proxy::_tao_any_destructor (void * p)
  {
    TAO_CORBA::remove_ref (static_cast<Valuetype_proxy*> (p));
  }

  TAO_CORBA::TypeCode_ptr Valuetype_proxy::_tao_type () const
  {
    // TODO: Should we retrieve typecode from X11 valuetype or
    // can we leave handling fully in X11 space?
    return {};
  }

  void Valuetype_proxy::truncation_hook ()
  {
    // take the address of the (static) repository id of the
    // associated valuetype (most derived) as marker for the
    // level in the inheritance hierarchy to truncate at
    this->truncation_mark_ = &this->value_->_obv_repository_id ();
  }

  const char * Valuetype_proxy::_tao_obv_repository_id () const
  {
    return this->value_->_obv_repository_id ().c_str ();
  }

  void Valuetype_proxy::_tao_obv_truncatable_repo_ids (Repository_Id_List &idlist) const
  {
    std::vector<std::string const*> _ids;
    this->value_->_obv_get_truncatable_repo_ids (_ids);
    for (std::string const* _id : _ids)
    {
      idlist.push_back (_id->c_str ());
    }
  }

  TAO_CORBA::Boolean Valuetype_proxy::_tao_match_formal_type (ptrdiff_t _t) const
  {
    return this->value_->_obv_match_formal_type (_t);
  }

  TAO_CORBA::Boolean Valuetype_proxy::_tao_marshal_v (TAO_OutputCDR &strm) const
  {
    return this->value_->_obv_marshal_v (strm);
  }

  TAO_CORBA::Boolean Valuetype_proxy::_tao_unmarshal_v (TAO_InputCDR & strm)
  {
    return this->value_->_obv_unmarshal_v (strm);
  }

  TAO_CORBA::ValueBase *
  ValueFactory_proxy::create_for_unmarshal ()
  {
    TAOX11_CORBA::valuetype_reference<TAOX11_CORBA::ValueBase> vtref =
        this->vfb_->create_for_unmarshal ();
    Valuetype_proxy_ptr vtprx = vtref->_get_proxy ();
    vtprx->lock_value ();
    return vtprx;
  }

  const char*
  ValueFactory_proxy::tao_repository_id ()
  {
    return this->vfb_->_obv_repository_id ().c_str ();
  }

} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBA::Boolean
operator<< (TAO_OutputCDR &strm, TAOX11_CORBA::ValueBase::_ref_type vb)
{
  return
    TAOX11_CORBA::ValueBase::_obv_marshal (
        strm,
        vb,
        reinterpret_cast<ptrdiff_t> (std::addressof(TAOX11_CORBA::ValueBase::_obv_unmarshal))
      );
}

TAO_CORBA::Boolean
operator>> (TAO_InputCDR &strm, TAOX11_CORBA::ValueBase::_ref_type &vb)
{
  return TAOX11_CORBA::ValueBase::_obv_unmarshal (strm, vb);
}

TAO_END_VERSIONED_NAMESPACE_DECL
