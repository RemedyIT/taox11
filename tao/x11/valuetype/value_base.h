/**
 * @file    value_base.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueType base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUETYPE_VALUE_BASE_H_INCLUDED
#define TAOX11_VALUETYPE_VALUE_BASE_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/valuetype/taox11_valuetype_export.h"
#include "tao/x11/valuetype/valuetype_traits_t.h"
#include "tao/x11/valuetype/abstractbase_traits_t.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/corba.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;
class TAO_InputCDR;
class TAO_ChunkInfo;

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{
  class TAOX11_DynCommon; // forward

  class Valuetype_proxy;
  using Valuetype_proxy_ptr = Valuetype_proxy *;
  class ValueFactory_proxy;

  namespace CORBA
  {
    class Any; // forward
    class AbstractBase; // forward
    class ValueFactoryBase; //forward

    class TAOX11_Valuetype_Export ValueBase
    {
    public:
      using _traits_type = CORBA::valuetype_traits<ValueBase>;
      using _ref_type = CORBA::valuetype_reference<ValueBase>;

      virtual valuetype_reference<ValueBase> _copy_value () const;

      static bool _obv_marshal (TAO_OutputCDR &strm,
                                _ref_type vt,
                                ptrdiff_t formal_type_id = 0);
      static bool _obv_unmarshal (TAO_InputCDR &strm,
                                  _ref_type& new_vt);

      /// Return typecode of most derived type.
      /// Used in CORBA::Any implementation.
      virtual CORBA::typecode_reference _obv_typecode () const;

    protected:
      using _shared_ptr_type = std::shared_ptr<CORBA::ValueBase>;

      template <typename T>
      friend class CORBA::valuetype_reference;
      friend class TAOX11_NAMESPACE::Valuetype_proxy;
      friend class TAOX11_NAMESPACE::ValueFactory_proxy;
      friend class CORBA::AbstractBase;
      friend class ValueFactoryBase;
      friend class TAOX11_NAMESPACE::TAOX11_DynCommon;

      ValueBase ();
      ValueBase (const ValueBase&);
      ValueBase (ValueBase&&);
      virtual ~ValueBase ();

      _ref_type _lock ()
      { return _ref_type (this); }

      _shared_ptr_type _lock_shared ();

      /// for use by valuetype skeletons
      bool _is_locked () const
      { return !this->self_ref_.expired (); }
      void _initialize_shared (_shared_ptr_type _spvt)
      {
        if (this->self_ref_.expired ())
          this->self_ref_ = _spvt;
      }

      void _set_truncatable (bool);
      bool _is_truncatable () const;
      bool _uses_chunking () const;

      /// Return the repository id for this (most derived) valuetype.
      virtual const std::string& _obv_repository_id () const = 0;
      /// Retrieve the list of repository ids in the valuetype "truncatable"
      /// inheritance hierarchy. List the id of this valuetype first
      /// and go up the "truncatable" derivation hierarchy.
      /// Note the truncatable repo ids only list the truncatable base types
      /// to which this type is safe to truncate, not all its parents.
      virtual void _obv_get_truncatable_repo_ids (std::vector<std::string const*>&) const = 0;

      virtual bool _obv_match_formal_type (ptrdiff_t ) const;

      virtual bool _obv_marshal_v (TAO_OutputCDR &) const = 0;
      virtual bool _obv_unmarshal_v (TAO_InputCDR &) = 0;

      /// Only overloaded in Valuetype classes inheriting from abstract
      /// interfaces.
      virtual abstractbase_reference<AbstractBase> _obv_abstract_base ();

      Valuetype_proxy_ptr _get_proxy ()
      { return this->proxy_; }

    private:
      class dtor
      {
      public:
        void operator ()(ValueBase* p)
        { delete p; }
      };
      friend class dtor;
      ValueBase& operator =(const ValueBase&) = delete;
      ValueBase& operator =(ValueBase&&) = delete;

      using _weak_reference_type = std::weak_ptr<ValueBase>;
      _weak_reference_type self_ref_;
      Valuetype_proxy_ptr proxy_;
    }; // ValueBase

    template<>
    struct TAOX11_Valuetype_Export valuetype_traits<CORBA::ValueBase>
    {
      using ref_type = valuetype_reference<CORBA::ValueBase>;
      using weak_ref_type = weak_valuetype_reference<CORBA::ValueBase>;

      static inline ref_type narrow (valuetype_reference<CORBA::ValueBase> vtref)
      {
        return vtref;
      }
    };

    extern TAOX11_Valuetype_Export CORBA::typecode_reference const _tc_ValueBase;
    extern TAOX11_Valuetype_Export CORBA::typecode_reference const _tc_EventBase;

  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits <CORBA::ValueBase> :
      public IDL::common_byval_traits <CORBA::valuetype_reference <CORBA::ValueBase>>,
      public CORBA::valuetype_traits <CORBA::ValueBase>
    {
      using is_abstract = std::true_type;
      using is_truncatable = std::false_type ;

      template <typename OStrm_, typename Formatter = formatter<CORBA::ValueBase, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };

    template <typename OStrm_>
    struct formatter< CORBA::ValueBase, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::ValueBase>::in_type o_)
      {
        os_ << "IDL::traits<CORBA::ValueBase>::ref_type{"
            << (o_ ? o_->_obv_typecode ()->id () : "nil") << "}";
        return os_;
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::ValueBase>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::ValueBase>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::ValueBase, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::ValueBase>::write_on (
          os, w.val_,
          formatter_t ());
    }
  } // namespace IDL

  // CORBA::Any insertion/extraction ops
  TAOX11_Valuetype_Export void
  operator<<= (CORBA::Any&, TAOX11_CORBA::ValueBase::_ref_type);

  TAOX11_Valuetype_Export bool
  operator>>= (const CORBA::Any&, TAOX11_CORBA::ValueBase::_ref_type &);
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_VALUETYPE_VALUE_BASE_H_INCLUDED
