/**
 * @file    abstract_base.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 Abstract interface base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUETYPE_ABSTRACT_BASE_H_INCLUDED
#define TAOX11_VALUETYPE_ABSTRACT_BASE_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/valuetype/taox11_valuetype_export.h"
#include "tao/x11/valuetype/value_base.h"
#include "tao/x11/valuetype/abstractbase_traits_t.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/corba.h"
#include "tao/x11/object.h"

namespace TAOX11_NAMESPACE
{
  class Abstractbase_proxy;
  using Abstractbase_proxy_ptr = Abstractbase_proxy*;
  class ValueFactory_proxy;

  namespace CORBA
  {
    class Any; // forward

    class TAOX11_Valuetype_Export AbstractBase
    {
    public:
      using _traits_type = abstractbase_traits<AbstractBase>;
      using _ref_type = abstractbase_reference<AbstractBase>;

      /// Overloaded in derived (object & valuetype) classes
      virtual IDL::traits<CORBA::Object>::ref_type _to_object ();
      virtual CORBA::valuetype_reference<CORBA::ValueBase> _to_value ();

      /// Check the (local) repository id (overloaded in derived interfaces).
      virtual bool _is_a (const std::string& local_type_id);

      /// Get the (local) repository id (overloaded in derived interfaces).
      virtual std::string _interface_repository_id () const;

      static bool _abs_marshal (TAO_OutputCDR&, _ref_type);
      static bool _abs_unmarshal (TAO_InputCDR&, _ref_type&);

    protected:
      using _shared_ptr_type = std::shared_ptr<CORBA::AbstractBase>;

      friend class TAOX11_NAMESPACE::ValueFactory_proxy;
      template <typename T> friend class abstractbase_reference;
#if (defined (_MSC_VER) && (_MSC_VER < 1910)) || (defined __clang__ && __clang_major__ <= 11)
      // Visual C++ 14.0 has a problem with the narrow method as friend
      // so make the full traits a friend, issue #4015
      // Clang doesn't support this friend construct, issue #4476
      template<typename T>
      friend struct TAOX11_CORBA::abstractbase_traits;
#else
      template<typename T>
      friend typename CORBA::abstractbase_traits<T>::ref_type
        CORBA::abstractbase_traits<T>::narrow (
          CORBA::abstractbase_reference<CORBA::AbstractBase>);
#endif

      AbstractBase () = default;
      explicit AbstractBase (Abstractbase_proxy_ptr prx);
      virtual ~AbstractBase ();
      /// Required for derived value types
      AbstractBase (const AbstractBase&);
      AbstractBase (AbstractBase&&);

      /// Overloaded in derived (interface & valuetype) classes
      virtual bool _is_object ();

      /// Overloaded in derived valuetype classes
      virtual const std::string& _obv_repository_id () const;
      virtual bool _obv_marshal_v (TAO_OutputCDR &) const;

      _ref_type _lock ()
      { return _ref_type (this); }

      /// Overridden in derived classes (either objects or valuetypes)
      _shared_ptr_type _lock_shared ();

      /// for use by derived classes (either objects or valuetypes)
      bool _is_locked () const
      { return !this->self_ref_.expired (); }
      void _initialize_shared (_shared_ptr_type _spvt)
      {
        if (this->self_ref_.expired ())
          this->self_ref_ = _spvt;
      }

    private:
      class dtor
      {
      public:
        void operator ()(AbstractBase* p)
        { delete p; }
      };
      friend class dtor;
      AbstractBase& operator =(const AbstractBase&) = delete;
      AbstractBase& operator =(AbstractBase&&) = delete;

      using _weak_reference_type = std::weak_ptr<AbstractBase>;
      _weak_reference_type self_ref_;
      Abstractbase_proxy_ptr proxy_ {};
    }; // AbstractBase

    extern TAOX11_Valuetype_Export CORBA::typecode_reference const _tc_AbstractBase;

    template<>
    inline abstractbase_traits<AbstractBase>::ref_type
    abstractbase_traits<AbstractBase>::narrow (
        abstractbase_reference<AbstractBase> abref)
    {
      return abref;
    }

  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits <CORBA::AbstractBase> :
      public IDL::common_byval_traits <CORBA::abstractbase_reference <CORBA::AbstractBase>>,
      public CORBA::abstractbase_traits <CORBA::AbstractBase>
    {
      template <typename OStrm_, typename Formatter = formatter<CORBA::AbstractBase, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }

      TAOX11_Valuetype_Export static std::string _dump (ref_type, const char*);
    };

    template <typename OStrm_>
    struct formatter< CORBA::AbstractBase, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::AbstractBase>::in_type o_)
      {
        return os_ << "IDL::traits<CORBA::AbstractBase>::ref_type {abstractbase:";
        TAOX11_IDL::traits<TAOX11_CORBA::Object>::ref_type obj_ref = o_->_to_object();

        if (obj_ref)
        {
          os_ << "object:";
#if (TAO_HAS_MINIMUM_CORBA == 0)
          os_ << obj_ref->_repository_id();
#endif
        }
        else //valuetype
        {
          TAOX11_IDL::traits<TAOX11_CORBA::ValueBase>::ref_type val_ref = o_->_to_value();
          if (val_ref)
          {
            os_ << "valuetype:" << val_ref->_obv_typecode ()->id ();
          }
          else
          {
            os_ << "nil";
          }
        }
        return os_ << "}";
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::AbstractBase>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::AbstractBase>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::AbstractBase, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::AbstractBase>::write_on (
          os, w.val_,
          formatter_t ());
    }
  } // namespace IDL

  // CORBA::Any insertion/extraction ops
  TAOX11_Valuetype_Export void
  operator<<= (CORBA::Any&, TAOX11_CORBA::AbstractBase::_ref_type);

  TAOX11_Valuetype_Export bool
  operator>>= (const CORBA::Any&, TAOX11_CORBA::AbstractBase::_ref_type &);
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_VALUETYPE_ABSTRACT_BASE_H_INCLUDED
