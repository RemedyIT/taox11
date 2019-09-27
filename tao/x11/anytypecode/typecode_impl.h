/**
 * @file    typecode_impl.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Typecode implementation
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_TYPECODE_IMPL_H
#define TAOX11_TYPECODE_IMPL_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/tao_corba.h"

#include "tao/x11/sequence_cdr_t.h"
#include "tao/x11/anytypecode/VisibilityC.h"
#include "tao/x11/anytypecode/ValueModifierC.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/any.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode; // forward TAO version
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    template<>
    TAOX11_AnyTypeCode_Export object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode_impl>::shared_ptr_type
    object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode_impl>::lock_shared (
        ::TAOX11_NAMESPACE::CORBA::TypeCode_impl*);

    class TAOX11_AnyTypeCode_Export TypeCode_impl
      : public IDL::traits<TypeCode>::base_type
    {
    public:
      TypeCode_impl (TAO_CORBA::TypeCode_ptr tc)
        : TypeCode (),
          static_ (false)
      { this->u_.tc_ = tc; }
      TypeCode_impl (TAO_CORBA::TypeCode* const &tc, bool /*static*/)
        : TypeCode (),
          static_ (true)
      { this->u_.tcref_ = const_cast<TAO_CORBA::TypeCode_ptr*> (std::addressof(tc)); }
      virtual ~TypeCode_impl ();

      virtual bool _is_a (const std::string& local_type_id) override;

      virtual bool equal (typecode_reference tc) const override;

      virtual bool equivalent (typecode_reference tc) const override;

      virtual TAOX11_CORBA::TCKind kind () const override;

      virtual typecode_reference get_compact_typecode () const override;

      virtual std::string id () const override;

      virtual std::string name () const override;

      virtual uint32_t member_count () const override;

      virtual std::string member_name (uint32_t index) const override;

      virtual typecode_reference member_type (uint32_t index) const override;
      virtual CORBA::Any member_label (uint32_t index) const override;

      virtual typecode_reference discriminator_type () const override;

      virtual int32_t default_index () const override;

      virtual uint32_t length () const override;

      virtual typecode_reference content_type () const override;

      virtual uint16_t fixed_digits () const override;

      virtual uint16_t fixed_scale () const override;

      virtual Visibility member_visibility (uint32_t index) const override;

      virtual ValueModifier type_modifier () const override;

      virtual typecode_reference concrete_base_type () const override;

      static bool _marshal (TAO_OutputCDR&, _ref_type);
      static bool _unmarshal (TAO_InputCDR&, _ref_type&);

      static inline TAO_CORBA::TypeCode_ptr get_tao_tc (CORBA::typecode_reference tcref)
      {
        return tcref->_impl ()->_tc ();
      }

      static CORBA::TCKind unaliased_kind (CORBA::typecode_reference);

    protected:
      friend struct object_traits<TypeCode_impl>;

      virtual bool _tc_marshal (TAO_OutputCDR&)  override;
      virtual bool _tc_unmarshal (TAO_InputCDR&) override;

      virtual TypeCode_impl* _impl () override;

    private:

      static const std::string _repository_id;

      bool static_;
      union
      {
        TAO_CORBA::TypeCode_ptr   tc_;
        TAO_CORBA::TypeCode_ptr * tcref_;
      } u_;

      inline const TAO_CORBA::TypeCode * _tc () const
      {
        return (this->static_ ? *this->u_.tcref_ : this->u_.tc_);
      }
      inline TAO_CORBA::TypeCode_ptr _tc ()
      {
        return (this->static_ ? *this->u_.tcref_ : this->u_.tc_);
      }
    };

  } // namespace CORBA

  typedef CORBA::TypeCode_impl TC_helper;

} // namespace TAOX11_NAMESPACE

TAOX11_AnyTypeCode_Export
std::ostream& operator<< (
    std::ostream& strm,
    TAOX11_NAMESPACE::IDL::traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type tc);

// In case declarations already seen (from Typecode_typesP.h included from Typecode_typesC.h
// included by some other *C.cpp) skip them here. MinGW issues warnings on repeated decls.
// When we compile typecode_impl.cpp Typecode_typesP.h will not be included so we use these
// decls which than set the correct export flags for the code compiled from typecode_impl.cpp.
#ifndef _INTF_TAOX11_NAMESPACE_CORBA_TYPECODE_CDR_OPS_

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean
operator<< (TAO_OutputCDR &, const TAOX11_NAMESPACE::CORBA::TypeCode::Bounds& );
TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean
operator>> (TAO_InputCDR &, TAOX11_NAMESPACE::CORBA::TypeCode::Bounds&);

TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean
operator<< (TAO_OutputCDR &, const TAOX11_NAMESPACE::CORBA::TypeCode::BadKind& );
TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean
operator>> (TAO_InputCDR &, TAOX11_NAMESPACE::CORBA::TypeCode::BadKind&);

TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean
operator<< (TAO_OutputCDR & cdr, TAOX11_CORBA::TypeCode::_ref_type tc);

TAOX11_AnyTypeCode_Export TAO_CORBA::Boolean
operator>> (TAO_InputCDR & cdr, TAOX11_CORBA::TypeCode::_ref_type& tc);

// CDR streaming helpers for typecode ref sequence
TAOX11_AnyTypeCode_Export bool
marshal_TAOX11_NAMESPACE_CORBA_TypeCode_sequence (
    TAO_OutputCDR& _strm,
    const std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound);
TAOX11_AnyTypeCode_Export bool
demarshal_TAOX11_NAMESPACE_CORBA_TypeCode_sequence (
    TAO_InputCDR& _strm,
    std::vector< TAOX11_CORBA::object_traits< ::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type>& _seq,
    uint32_t _bound);

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _INTF_TAOX11_NAMESPACE_CORBA_TYPECODE_CDR_OPS_ */

#endif /* TAOX11_TYPECODE_IMPL_H */
