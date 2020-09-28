/**
 * @file    valuetype_proxies.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueType TAO proxy classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUETYPE_VALUETYPE_PROXIES_H_INCLUDED
#define TAOX11_VALUETYPE_VALUETYPE_PROXIES_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/tao_corba.h"
#include "tao/Valuetype/AbstractBase.h"
#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/ValueFactory.h"
#include "tao/x11/valuetype/taox11_valuetype_export.h"
#include "tao/x11/valuetype/value_base.h"
#include "tao/x11/valuetype/value_factory_base.h"

namespace TAOX11_NAMESPACE
{

  //-------------------------------------------------------------------
  //  Valuetype proxy class
  //
  //  Wrapper class for reference to TAOX11 Value instance to enable
  //  to marshal/demarshal to/from TAO CDR streams.
  //===================================================================

  class Valuetype_proxy;
  using Valuetype_proxy_ptr = Valuetype_proxy*;
  typedef
    TAO_VERSIONED_NAMESPACE_NAME::TAO_Value_Var_T<
      Valuetype_proxy
      >
    Valuetype_proxy_var;

  typedef
    TAO_VERSIONED_NAMESPACE_NAME::TAO_Value_Out_T<
      Valuetype_proxy
      >
    Valuetype_proxy_out;

  class TAOX11_Valuetype_Export Valuetype_proxy
      : public TAO_CORBA::DefaultValueRefCountBase
  {
    public:
      Valuetype_proxy (TAOX11_CORBA::ValueBase* vtp)
        : value_ (vtp), truncation_mark_ (nullptr)
      {}
      ~Valuetype_proxy () = default;

      // Noop; not used.
      TAO_CORBA::ValueBase* _copy_value () override;

      /// Used in the implementation of CORBA::Any
      static void _tao_any_destructor (void *);
      TAO_CORBA::TypeCode_ptr _tao_type () const override;

      virtual void truncation_hook ();

      static Valuetype_proxy* _downcast (TAO_CORBA::ValueBase *v)
      { return dynamic_cast<Valuetype_proxy*> (v); }

      /// Return the repository id of this valuetype.
      const char * _tao_obv_repository_id () const override;

      /// Give the list of the RepositoryIds in the valuetype "truncatable"
      /// inheritance hierarchy. List the id of this valuetype as first
      /// RepositoryID and go up the "truncatable" derivation hierarchy.
      /// Note the truncatable repo ids only list the truncatable base types
      /// to which this type is safe to truncate, not all its parents.
      void _tao_obv_truncatable_repo_ids (Repository_Id_List &) const override;

      TAO_CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) const override;
      TAO_CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &) override;

      TAO_CORBA::Boolean _tao_match_formal_type (ptrdiff_t ) const override;

      TAOX11_CORBA::ValueBase* get_value () const
      { return this->value_; }

      TAOX11_CORBA::ValueBase::_ref_type get_locked_value ()
      {
        if (this->locked_value_)
          return this->release_value ();
        else
          return this->value_->_lock ();
      }

      bool needs_truncation ()
      { return this->truncation_mark_ == &this->value_->_obv_repository_id (); }

    protected:
      friend class ValueFactory_proxy;
      friend class CORBA::ValueBase;
      friend class CORBA::AbstractBase;

      // For use by ValueFactory_proxy to lock the X11 valuetype reference
      // inside the Valuetype_proxy to keep it alive until returned to the
      // X11 code which started the extraction.
      void lock_value ()
      { this->locked_value_ = this->value_->_lock (); }

      // To unlock the X11 valuetype reference after successful extraction.
      TAOX11_CORBA::ValueBase::_ref_type release_value ()
      {
        TAOX11_CORBA::ValueBase::_ref_type retref;
        std::swap (this->locked_value_, retref);
        return retref;
      }

    private:
      using truncation_mark_t = std::string const*;

      TAOX11_CORBA::ValueBase* value_;
      TAOX11_CORBA::ValueBase::_ref_type locked_value_;
      truncation_mark_t truncation_mark_;
  };

  //-------------------------------------------------------------------
  //  Valuefactory_proxy class
  //
  //  Wrapper class for reference to TAOX11 valuefactory instance
  //===================================================================

  class ValueFactory_proxy
    : public TAO_CORBA::ValueFactoryBase
  {
    public:
      ValueFactory_proxy (CORBA::ValueFactoryBase::_ref_type vfb)
        : vfb_ (vfb)
      {}

      static ValueFactory_proxy* _downcast ( TAO_CORBA::ValueFactoryBase *vfp)
      { return dynamic_cast<ValueFactory_proxy*> (vfp); }

      TAO_CORBA::ValueBase *
      create_for_unmarshal () override;

      // TAO-specific extensions
      virtual const char* tao_repository_id ();

      CORBA::ValueFactoryBase::_ref_type get_factory ()
      { return this->vfb_; }

    protected:
      ~ValueFactory_proxy () = default;

    private:
      CORBA::ValueFactoryBase::_ref_type vfb_;
  };

} // namespace TAOX11_NAMESPACE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<>
  struct  Value_Traits<TAOX11_NAMESPACE::Valuetype_proxy>
  {
    static void add_ref (TAOX11_NAMESPACE::Valuetype_proxy *);
    static void remove_ref (TAOX11_NAMESPACE::Valuetype_proxy *);
    static void release (TAOX11_NAMESPACE::Valuetype_proxy *);
  };
} // namespace TAO

TAOX11_Valuetype_Export TAO_CORBA::Boolean
operator<< (TAO_OutputCDR &, TAOX11_CORBA::ValueBase::_ref_type);

TAOX11_Valuetype_Export TAO_CORBA::Boolean
operator>> (TAO_InputCDR &, TAOX11_CORBA::ValueBase::_ref_type &);

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_VALUETYPE_VALUETYPE_PROXIES_H_INCLUDED
