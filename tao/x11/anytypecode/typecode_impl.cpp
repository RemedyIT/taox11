/**
 * @file    typecode_impl.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Typecode implementation
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/Struct_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"

#include "tao/x11/stddef.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"
#include "tao/x11/object_traits_t.h"
#include "tao/x11/anytypecode/any_util.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/anytypecode/any_dual_impl_t.h"
#include "tao/x11/anytypecode/any_basic_impl_t.h"

std::ostream& operator<< (
    std::ostream& strm,
    TAOX11_NAMESPACE::IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type tc)
{
  return TAOX11_NAMESPACE::IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode>::write_on (strm, tc);
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBA::Boolean
operator<< (TAO_OutputCDR & cdr, TAOX11_CORBA::TypeCode::_ref_type tc)
{
  return TAOX11_CORBA::TypeCode_impl::_marshal (cdr, tc);
}

TAO_CORBA::Boolean
operator>> (TAO_InputCDR & cdr, TAOX11_CORBA::TypeCode::_ref_type& tc)
{
  return TAOX11_CORBA::TypeCode_impl::_unmarshal (cdr, tc);
}

// CDR streaming helpers for typecode ref sequence
bool
marshal_TAOX11_NAMESPACE_CORBA_TypeCode_sequence (
    TAO_OutputCDR& _strm,
    const std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound)
{
  using _elem_type = IDL::traits<TAOX11_CORBA::TypeCode>::ref_type;

  uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

  if (_bound > 0 && length > _bound)
    return false;

  if (! (_strm << length))
  {
    return false;
  }

  for (const _elem_type& _el : _seq)
  {
    if (! (_strm << _el))
    {
      return false;
    }
  }

  return true;
}

bool
demarshal_TAOX11_NAMESPACE_CORBA_TypeCode_sequence (
    TAO_InputCDR& _strm,
    std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound)
{
  using _elem_type = IDL::traits<TAOX11_CORBA::TypeCode>::ref_type;

  uint32_t length {};

  if (! (_strm >> length))
  {
    return false;
  }

  if (_bound > 0 && length > _bound)
    return false;

  _seq.resize (length);

  for (_elem_type& _el : _seq)
  {
    if (! (_strm >> _el))
    {
      return false;
    }
  }

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    template<>
    object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode_impl>::shared_ptr_type
    object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode_impl>::lock_shared (
        ::TAOX11_NAMESPACE::CORBA::TypeCode_impl* p)
    {
      return std::dynamic_pointer_cast< ::TAOX11_NAMESPACE::CORBA::TypeCode_impl> (p->_lock_shared ());
    }

    bool TypeCode_impl::_marshal (TAO_OutputCDR& strm, _ref_type tc)
    {
      return tc->_tc_marshal (strm);
    }

    bool TypeCode_impl::_unmarshal (TAO_InputCDR& strm, _ref_type& tc)
    {
      _ref_type new_tc = make_reference<TypeCode_impl> (nullptr);
      if (new_tc->_tc_unmarshal (strm))
      {
        tc = new_tc;
        return true;
      }
      return false;
    }

    TypeCode_impl::~TypeCode_impl ()
    {
      if (!this->static_) TAO_CORBA::release (this->_tc ());
    }

    bool TypeCode_impl::_tc_marshal (TAO_OutputCDR& strm)
    {
      return (strm << this->_tc ());
    }

    bool TypeCode_impl::_tc_unmarshal (TAO_InputCDR& strm)
    {
      if (this->_tc () != nullptr)
      {
        TAO_CORBA::release (this->_tc ());
      }
      this->u_.tc_ = nullptr;
      return (strm >> this->u_.tc_);
    }

    TypeCode_impl* TypeCode_impl::_impl ()
    {
      return this;
    }

    const std::string TypeCode_impl::_repository_id ("IDL:omg.org/CORBA/TypeCode:1.0");

    bool TypeCode_impl::_is_a (const std::string& local_type_id)
    {
      return (
        local_type_id == _repository_id ||
        TAOX11_NAMESPACE::CORBA::LocalObject::_is_a (local_type_id));
    }

    bool TypeCode_impl::equal (typecode_reference tc) const
    {
      try
      {
        return this->_tc ()->equal (tc->_impl ()->_tc ());
      }
      catch_tao_system_ex (_sx)
      return false;
    }

    bool TypeCode_impl::equivalent (typecode_reference tc) const
    {
      try
      {
        return this->_tc ()->equivalent (tc->_impl ()->_tc ());
      }
      catch_tao_system_ex (_sx)
      return false;
    }

    TAOX11_CORBA::TCKind TypeCode_impl::kind () const
    {
      return static_cast<TAOX11_CORBA::TCKind> (this->_tc ()->kind ());
    }

    typecode_reference TypeCode_impl::get_compact_typecode () const
    {
      try
      {
        return make_reference<TypeCode_impl> (this->_tc ()->get_compact_typecode ());
      }
      catch_tao_system_ex (_sx)
      return typecode_reference ();
    }

    std::string TypeCode_impl::id () const
    {
      try
      {
        TAO_CORBA::String_var _id = this->_tc ()->id();
        return (_id.in());
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return std::string();
    }

    std::string TypeCode_impl::name () const
    {
      try
      {
        TAO_CORBA::String_var _name = this->_tc ()->name();
        return (_name.in());
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
       throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return std::string();
    }

    uint32_t TypeCode_impl::member_count () const
    {
      try
      {
        return this->_tc ()->member_count();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    std::string TypeCode_impl::member_name (uint32_t index) const
    {
      try
      {
        TAO_CORBA::String_var _name = this->_tc ()->member_name( index);
        return (_name.in());
      }
      catch (const TAO_CORBA::TypeCode::Bounds&) {
        throw TypeCode::Bounds ();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return std::string();
    }

    typecode_reference TypeCode_impl::member_type (uint32_t index) const
    {
      try
      {
        return make_reference<TypeCode_impl> (this->_tc ()->member_type ( index));
      }
      catch (const TAO_CORBA::TypeCode::Bounds&) {
        throw TypeCode::Bounds ();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return make_reference<TypeCode_impl> (nullptr);
    }

    CORBA::Any TypeCode_impl::member_label (uint32_t index) const
    {
      CORBA::Any any;
      try
      {
        TAO_CORBA::Any_var tao_any = this->_tc ()-> member_label (index);
        Any_Util::tao_any_to_x11_any (tao_any.in (), any);
        return any;
      }
      catch (const TAO_CORBA::TypeCode::Bounds&) {
        throw TypeCode::Bounds ();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return any;
    }

    typecode_reference TypeCode_impl::discriminator_type () const
    {
      try
      {
        return make_reference<TypeCode_impl> (this->_tc ()->discriminator_type ());
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return make_reference<TypeCode_impl> (nullptr);
    }

    int32_t TypeCode_impl::default_index () const
    {
      try
      {
        return this->_tc ()->default_index();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    uint32_t TypeCode_impl::length () const
    {
      try
      {
        return this->_tc ()->length();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    typecode_reference TypeCode_impl::content_type () const
    {
      try
      {
        return make_reference<TypeCode_impl> (this->_tc ()->content_type ());
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return make_reference<TypeCode_impl> (nullptr);
    }

    uint16_t TypeCode_impl::fixed_digits () const
    {
      try
      {
        return this->_tc ()->fixed_digits();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    uint16_t TypeCode_impl::fixed_scale () const
    {
      try
      {
        return this->_tc ()->fixed_scale ();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    Visibility TypeCode_impl::member_visibility (uint32_t index) const
    {
      try
      {
        return this->_tc ()->member_visibility (index);
      }
      catch (const TAO_CORBA::TypeCode::Bounds&) {
        throw TypeCode::Bounds ();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    ValueModifier TypeCode_impl::type_modifier () const
    {
      try
      {
        return this->_tc ()->type_modifier ();
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return 0;
    }

    typecode_reference TypeCode_impl::concrete_base_type () const
    {
      try
      {
        return make_reference<TypeCode_impl> (this->_tc ()->concrete_base_type ());
      }
      catch (const TAO_CORBA::TypeCode::BadKind&) {
        throw TypeCode::BadKind ();
      }
      catch_tao_system_ex(_ex)
      return make_reference<TypeCode_impl> (nullptr);
    }

    CORBA::TCKind TypeCode_impl::unaliased_kind (CORBA::typecode_reference tc)
    {
      return static_cast<CORBA::TCKind> (TAO_TAO::unaliased_kind (get_tao_tc (tc)));
    }

  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

namespace __tao
{
  namespace TAOX11_NAMESPACE
  {
    namespace CORBA
    {
      namespace TypeCode
      {
        static TAO_TAO::TypeCode::Struct<
          char const *,
          TAO_CORBA::TypeCode_ptr const *,
          TAO_TAO::TypeCode::Struct_Field<
            char const *,
            TAO_CORBA::TypeCode_ptr const *> const *,
          TAO_TAO::Null_RefCount_Policy>
          _tao_tc_Bounds (
          TAO_CORBA::tk_except,
          "IDL:omg.org/CORBA/TypeCode/Bounds:1.0",
          "Bounds",
          nullptr,
          0);

        TAO_CORBA::TypeCode_ptr const _tc_Bounds = &_tao_tc_Bounds;

        static TAO_TAO::TypeCode::Struct<
          char const *,
          TAO_CORBA::TypeCode_ptr const *,
          TAO_TAO::TypeCode::Struct_Field<
            char const *,
            TAO_CORBA::TypeCode_ptr const *> const *,
          TAO_TAO::Null_RefCount_Policy>
        _tao_tc_BadKind (
          TAO_CORBA::tk_except,
          "IDL:omg.org/CORBA/TypeCode/BadKind:1.0",
          "BadKind",
          nullptr,
          0);

        TAO_CORBA::TypeCode_ptr const _tc_BadKind = &_tao_tc_BadKind;
      }  // namespace TypeCode_impl
    } // namespace CORBA
  } // namespace TAOX11_NAMESPACE
} // namespace __tao

TAOX11_NAMESPACE::CORBA::typecode_reference const TAOX11_NAMESPACE::CORBA::TypeCode::_tc_Bounds =
  TAOX11_NAMESPACE::CORBA::make_reference<TAOX11_NAMESPACE::CORBA::TypeCode_impl> (
    __tao::TAOX11_NAMESPACE::CORBA::TypeCode::_tc_Bounds, true);

TAOX11_NAMESPACE::CORBA::typecode_reference const TAOX11_NAMESPACE::CORBA::TypeCode::_tc_BadKind =
  TAOX11_NAMESPACE::CORBA::make_reference<TAOX11_NAMESPACE::CORBA::TypeCode_impl> (
    __tao::TAOX11_NAMESPACE::CORBA::TypeCode::_tc_BadKind, true);


void
TAOX11_NAMESPACE::CORBA::TypeCode::Bounds::_info (std::ostream& user_exception_info) const
{
  TAOX11_IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode::Bounds>::write_on(user_exception_info, *this);
}

void TAOX11_NAMESPACE::CORBA::TypeCode::Bounds::_raise () const
{
  throw *this;
}

void TAOX11_NAMESPACE::CORBA::TypeCode::Bounds::_tao_encode (TAO_OutputCDR &cdr) const
{
  if (!(cdr << *this))
  {
    throw TAO_CORBA::MARSHAL ();
  }
}

void TAOX11_NAMESPACE::CORBA::TypeCode::Bounds::_tao_decode (TAO_InputCDR &cdr)
{
  if (!(cdr >> *this))
  {
    throw TAO_CORBA::MARSHAL ();
  }
}

TAOX11_NAMESPACE::CORBA::Exception *TAOX11_NAMESPACE::CORBA::TypeCode::Bounds::_tao_duplicate () const
{
  TAOX11_NAMESPACE::CORBA::Exception * result = nullptr;
  ACE_NEW_NORETURN (result, Bounds (*this));
  if (!result)
    throw TAO_CORBA::NO_MEMORY ();
  return result;
}

void
TAOX11_NAMESPACE::CORBA::TypeCode::BadKind::_info (std::ostream& user_exception_info) const
{
  TAOX11_IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode::BadKind>::write_on(user_exception_info, *this);
}

void TAOX11_NAMESPACE::CORBA::TypeCode::BadKind::_raise () const
{
  throw *this;
}

void TAOX11_NAMESPACE::CORBA::TypeCode::BadKind::_tao_encode (TAO_OutputCDR &cdr) const
{
  if (!(cdr << *this))
  {
    throw TAO_CORBA::MARSHAL ();
  }
}

void TAOX11_NAMESPACE::CORBA::TypeCode::BadKind::_tao_decode (TAO_InputCDR &cdr)
{
  if (!(cdr >> *this))
  {
    throw TAO_CORBA::MARSHAL ();
  }
}

TAOX11_NAMESPACE::CORBA::Exception *TAOX11_NAMESPACE::CORBA::TypeCode::BadKind::_tao_duplicate () const
{
  TAOX11_NAMESPACE::CORBA::Exception * result = nullptr;
  ACE_NEW_NORETURN (result, BadKind (*this));
  if (!result)
    throw TAO_CORBA::NO_MEMORY ();
  return result;
}

// copying version
void operator<<= (TAOX11_CORBA::Any &any, const ::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds& _struct )
{
  TAOX11_NAMESPACE::Any_Dual_Impl_T<
      IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds>,
      TAOX11_NAMESPACE::CDR_Marshal_Policy
        >::insert_copy (
          any,
          TAOX11_NAMESPACE::CORBA::TypeCode::_tc_Bounds,
          _struct);
}

// non copying version
void operator<<= (TAOX11_CORBA::Any &any , ::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds&& _struct )
{
   Any_Dual_Impl_T<
      IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds>,
      TAOX11_NAMESPACE::CDR_Marshal_Policy
        >::insert (
          any,
          TAOX11_NAMESPACE::CORBA::TypeCode::_tc_Bounds,
          std::move(_struct));
}

// extraction
bool operator>>= (const TAOX11_CORBA::Any &any , ::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds& _struct)
{
  return  Any_Dual_Impl_T<
      IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds>,
      TAOX11_NAMESPACE::CDR_Marshal_Policy
        >::extract (
          any,
          TAOX11_NAMESPACE::CORBA::TypeCode::_tc_Bounds,
          _struct);
}

// copying version
void operator<<= (TAOX11_CORBA::Any &any, const ::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind& _struct )
{
  TAOX11_NAMESPACE::Any_Dual_Impl_T<
      IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind>,
      TAOX11_NAMESPACE::CDR_Marshal_Policy
        >::insert_copy (
          any,
          TAOX11_NAMESPACE::CORBA::TypeCode::_tc_BadKind,
          _struct);
}

// non copying version
void operator<<= (TAOX11_CORBA::Any &any , ::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind&& _struct )
{
   Any_Dual_Impl_T<
      IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind>,
      TAOX11_NAMESPACE::CDR_Marshal_Policy
        >::insert (
          any,
          TAOX11_NAMESPACE::CORBA::TypeCode::_tc_BadKind,
          std::move(_struct));
}

// extraction
bool operator>>= (const TAOX11_CORBA::Any &any , ::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind& _struct)
{
  return  Any_Dual_Impl_T<
      IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind>,
      TAOX11_NAMESPACE::CDR_Marshal_Policy
        >::extract (
          any,
          TAOX11_NAMESPACE::CORBA::TypeCode::_tc_BadKind,
          _struct);
}


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const TAOX11_NAMESPACE::CORBA::TypeCode::Bounds &_tao_aggregate)
{
  // First marshal the repository ID.
  if (strm << _tao_aggregate._rep_id ())
  {
    return true;
  }
  else
  {
    return false;
  }
}

TAO_CORBA::Boolean operator>> (
    TAO_InputCDR &,
    TAOX11_NAMESPACE::CORBA::TypeCode::Bounds &)
{
  return true;
}

TAO_CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const TAOX11_NAMESPACE::CORBA::TypeCode::BadKind &_tao_aggregate)
{
  // First marshal the repository ID.
  if (strm << _tao_aggregate._rep_id ())
  {
    return true;
  }
  else
  {
    return false;
  }
}

TAO_CORBA::Boolean operator>> (
    TAO_InputCDR &,
    TAOX11_NAMESPACE::CORBA::TypeCode::BadKind &)
{
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL

