/**
 * @file    PolicyC.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 Policy client stub class
 * Handcrafted after initial generation by RIDL.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
/*
 * ****  Code generated by the RIDL Compiler ****
 * RIDL has been developed by:
 *        Remedy IT Expertise BV
 *        The Netherlands
 *        https://www.remedy.nl
 */

#define __TAOX11_INCLUDE_STUB_PROXY__

#include "tao/CDR.h"
#include "tao/Invocation_Adapter.h"
#include "tao/Object_T.h"

#include "tao/x11/basic_arguments.h"
#include "tao/x11/sequence_cdr_t.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/stub_arg_traits.h"

#include "PolicyC.h"

// generated from StubSourceWriter#enter_module
namespace TAOX11_NAMESPACE {
  namespace CORBA {

    // generated from c++/cli_src/except_src.erb
    const char* TAOX11_CORBA::PolicyError::what() const noexcept
    {
      return "TAOX11_CORBA::PolicyError";
    }

    void
    TAOX11_CORBA::PolicyError::_info (std::ostream& strm) const
    {
      TAOX11_IDL::traits<TAOX11_CORBA::PolicyError>::write_on(strm, *this);
    }

    void TAOX11_CORBA::PolicyError::_raise () const
    {
      throw *this;
    }

    void TAOX11_CORBA::PolicyError::_tao_encode (TAO_OutputCDR &cdr) const
    {
      if (!(cdr << *this))
      {
        throw TAO_CORBA::MARSHAL ();
      }
    }

    void TAOX11_CORBA::PolicyError::_tao_decode (TAO_InputCDR &cdr)
    {
      if (!(cdr >> *this))
      {
        throw TAO_CORBA::MARSHAL ();
      }
    }

    TAOX11_CORBA::Exception *TAOX11_CORBA::PolicyError::_tao_duplicate () const
    {
      TAOX11_NAMESPACE::CORBA::Exception * result = new (std::nothrow) PolicyError (*this);
      if (!result)
        throw TAO_CORBA::NO_MEMORY ();
      return result;
    }

    // generated from c++/cli_src/except_src.erb
    const char* TAOX11_CORBA::InvalidPolicies::what() const noexcept
    {
      return "TAOX11_CORBA::InvalidPolicies";
    }

    void
    TAOX11_CORBA::InvalidPolicies::_info (std::ostream& strm) const
    {
      TAOX11_IDL::traits<TAOX11_CORBA::InvalidPolicies>::write_on(strm, *this);
    }

    void TAOX11_CORBA::InvalidPolicies::_raise () const
    {
      throw *this;
    }

    void TAOX11_CORBA::InvalidPolicies::_tao_encode (TAO_OutputCDR &cdr) const
    {
      if (!(cdr << *this))
      {
        throw TAO_CORBA::MARSHAL ();
      }
    }

    void TAOX11_CORBA::InvalidPolicies::_tao_decode (TAO_InputCDR &cdr)
    {
      if (!(cdr >> *this))
      {
        throw TAO_CORBA::MARSHAL ();
      }
    }

    TAOX11_CORBA::Exception *TAOX11_CORBA::InvalidPolicies::_tao_duplicate () const
    {
      TAOX11_NAMESPACE::CORBA::Exception * result = new (std::nothrow) InvalidPolicies (*this);
      if (!result)
        throw TAO_CORBA::NO_MEMORY ();
      return result;
    }

    // generated from c++/cli_src/interface_pre.erb
    static const std::string TAOX11_NAMESPACE_corba_policy_stub_repo_id_ = "IDL:CORBA/Policy:1.0";

    std::string Policy::_interface_repository_id () const
    {
      return TAOX11_NAMESPACE_corba_policy_stub_repo_id_;
    }

    TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::ref_type
    Policy::_narrow (TAOX11_CORBA::object_traits< TAOX11_CORBA::Object>::ref_type obj)
    {
      return TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::narrow (obj);
    }

    // generated from c++/cli_src/attribute.erb
    TAOX11_CORBA::PolicyType Policy::policy_type()
    {
      try {
        TAO_CORBA::Policy_var _pp = TAO_CORBA::Policy::_narrow (this->get_proxy ()->get ());
        return static_cast< TAOX11_CORBA::PolicyType> (_pp->policy_type ());
      }
      catch_tao_system_ex(_ex)
      return TAOX11_CORBA::PolicyType ();
    }

    // generated from c++/cli_src/operation.erb
    TAOX11_CORBA::object_traits<Policy>::ref_type Policy::copy ()
    {
      return this->copy_i ();
    }

    TAOX11_CORBA::object_traits<Policy>::ref_type Policy::copy_i ()
    {
      try {
        TAO_CORBA::Policy_var _pp = TAO_CORBA::Policy::_narrow (this->get_proxy ()->get ());
        TAO_CORBA::Policy_ptr _pp_copy = _pp->copy ();
        if (!TAO_CORBA::is_nil (_pp_copy))
        {
          return CORBA::make_reference<Policy> (new TAOX11_NAMESPACE::Object_proxy (_pp_copy));
        }
      }
      catch_tao_system_ex(_ex)
      return object_traits<Policy>::ref_type ();
    }

    // generated from c++/cli_src/operation.erb
    void Policy::destroy ()
    {
      try {
        TAO_CORBA::Policy_var _pp = TAO_CORBA::Policy::_narrow (this->get_proxy ()->get ());
        _pp->destroy ();
      }
      catch_tao_system_ex(_ex)
    }

    // generated from c++/cli_src/interface_post.erb
    Policy::Policy (TAOX11_NAMESPACE::Object_proxy_ptr p) :
      Object (p)
    {
    }
    Policy::Policy () :
      Object ()
    {
    }

  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

// generated from StubSourceTraitsWriter#enter_module
namespace TAOX11_NAMESPACE {
  namespace CORBA {

    template<>
    TAOX11_Export object_traits< TAOX11_CORBA::Policy>::shared_ptr_type
    object_traits< TAOX11_CORBA::Policy>::lock_shared (
        TAOX11_CORBA::Policy* p)
    {
      return std::dynamic_pointer_cast< TAOX11_CORBA::Policy> (p->_lock_shared ());
    }

    /**
      * Since policies are local objects, we expect that 'objref'
      * always contains a policy base type.
      * We need to check this before we're using the 'objref' for narrowing
      * in TAO.
      * TAO will otherwise throw a transient exception if the supplied
      * 'objref' is not related to a policy.
      */
    template<>
    object_traits< TAOX11_CORBA::Policy>::ref_type
    object_traits< TAOX11_CORBA::Policy>::narrow (
        object_traits< TAOX11_CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        TAO_CORBA::Policy_ptr tmp = dynamic_cast<TAO_CORBA::Policy_ptr>(
          objref->get_proxy ()->get ());
        if (TAO_CORBA::is_nil (tmp))
          return nullptr;

        try
        {
          TAO_CORBA::Policy_ptr _pp =
              TAO_CORBA::Policy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (_pp))
          {
            return IDL::traits<Policy>::ref_type (
              new TAOX11_CORBA::Policy (
                new TAOX11_NAMESPACE::Object_proxy (_pp)));
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

// generated from StubSourceCDRWriter#pre_visit
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// generated from c++/cli_src/except_cdr.erb
TAO_CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const TAOX11_CORBA::PolicyError &_tao_aggregate)
{
  // First marshal the repository ID.
  if (strm << _tao_aggregate._rep_id ())
  {
    return
      (strm << _tao_aggregate.reason ());
  }
  else
  {
    return false;
  }
}

TAO_CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    TAOX11_CORBA::PolicyError &_tao_aggregate)
{
  return
    (strm >> _tao_aggregate.reason ());
}

// generated from c++/cli_src/except_cdr.erb
TAO_CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const TAOX11_CORBA::InvalidPolicies &_tao_aggregate)
{
  // First marshal the repository ID.
  if (strm << _tao_aggregate._rep_id ())
  {
    return
      (strm << _tao_aggregate.indices ());
  }
  else
  {
    return false;
  }
}

TAO_CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    TAOX11_CORBA::InvalidPolicies &_tao_aggregate)
{
  return
    (strm >> _tao_aggregate.indices ());
}

// generated from c++/cli_src/interface_cdr.erb

// CDR streaming operator specializations for TAOX11_CORBA::Policy

TAO_CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    IDL::traits<TAOX11_CORBA::Policy>::ref_type _tao_objref)
{
  IDL::traits<TAOX11_CORBA::Object>::ref_type _x11_obj = _tao_objref;
  return (strm << _x11_obj);
}

TAO_CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::ref_type& _tao_objref)
{
  TAO_CORBA::Policy_ptr _pol_ptr;

  if (!(strm >> _pol_ptr))
    {
      return false;
    }

  _tao_objref = TAOX11_CORBA::make_reference< TAOX11_CORBA::Policy> (
      new TAOX11_NAMESPACE::Object_proxy (_pol_ptr));

  return true;
}

// CDR streaming helpers for object ref sequence
bool marshal_TAOX11_NAMESPACE_CORBA_Policy_sequence (
    TAO_OutputCDR& _strm, const std::vector< TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::ref_type>& _seq, uint32_t _bound)
{
  using _elem_type = TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::ref_type;

  uint32_t length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

  if (_bound > 0 && length > _bound)
    return false;

  if (!(_strm << length))
  {
    return false;
  }

  for (const _elem_type& _el : _seq)
  {
    if (!(_strm << _el))
    {
      return false;
    }
  }

  return true;
}

bool demarshal_TAOX11_NAMESPACE_CORBA_Policy_sequence (
    TAO_InputCDR& _strm, std::vector< TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::ref_type>& _seq, uint32_t _bound)
{
  using _elem_type = TAOX11_CORBA::object_traits< TAOX11_CORBA::Policy>::ref_type;

  uint32_t length {};

  if (!(_strm >> length))
  {
    return false;
  }

  if (_bound > 0 && length > _bound)
    return false;

  _seq.resize (length);

  for (_elem_type& _el : _seq)
  {
    if (!(_strm >> _el))
    {
      return false;
    }
  }

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// generated from c++/cli_src/interface_os.erb
std::ostream& operator<< (
    std::ostream& strm ,
    IDL::traits<TAOX11_CORBA::Policy>::ref_type _v)
{
  strm << "object_reference {";
  if (_v == nullptr)
    strm << "nil";
  else
    strm << "'" << _v->_repository_id() << "'";
  return strm << "}";
}

// -*- END -*-
