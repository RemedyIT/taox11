/**
 * @file    messaging_policy_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   x11 CORBA Messaging RT Policy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/messaging/messaging_policy_impl.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace Messaging_impl
  {
    /*--------------------------------------------------------------------
     * RelativeRoundtripTimeoutPolicy
    --------------------------------------------------------------------*/
    RelativeRoundtripTimeoutPolicy::RelativeRoundtripTimeoutPolicy (
        TAO_MESSAGING::RelativeRoundtripTimeoutPolicy_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }

    ::TAOX11_NAMESPACE::TimeBase::TimeT
    RelativeRoundtripTimeoutPolicy::relative_expiry()
    {
      return static_cast< ::TAOX11_NAMESPACE::TimeBase::TimeT> (
        this->pol_->relative_expiry ());
    }

    /*--------------------------------------------------------------------
     * SyncScopePolicy
    --------------------------------------------------------------------*/
    SyncScopePolicy::SyncScopePolicy (TAO_MESSAGING::SyncScopePolicy_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }

    ::TAOX11_NAMESPACE::Messaging::SyncScope
    SyncScopePolicy::synchronization ()
    {
      return static_cast< ::TAOX11_NAMESPACE::Messaging::SyncScope> (
        this->pol_->synchronization ());
    }

    /*--------------------------------------------------------------------
     * ConnectionTimeoutPolicy
    --------------------------------------------------------------------*/
    ConnectionTimeoutPolicy::ConnectionTimeoutPolicy (
          TAO_TAO::ConnectionTimeoutPolicy_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }

    ::TAOX11_NAMESPACE::TimeBase::TimeT
    ConnectionTimeoutPolicy::relative_expiry()
    {
      return static_cast< ::TAOX11_NAMESPACE::TimeBase::TimeT> (
        this->pol_->relative_expiry ());
    }

    /*--------------------------------------------------------------------
     * BufferingConstraintPolicy
    --------------------------------------------------------------------*/
    BufferingConstraintPolicy::BufferingConstraintPolicy (
          TAO_TAO::BufferingConstraintPolicy_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }

    ::TAOX11_NAMESPACE::TAO::BufferingConstraint
    BufferingConstraintPolicy::buffering_constraint ()
    {
      TAO_TAO::BufferingConstraint tmp = this->pol_->buffering_constraint();
      return ::TAOX11_NAMESPACE::TAO::BufferingConstraint (
        tmp.mode,
        tmp.timeout,
        tmp.message_count,
        tmp.message_bytes);
    }
  }

  namespace CORBA
  {
    // RelativeRoundtripTimeoutPolicy
    template<>
    TAOX11_MESSAGING_Export object_traits< ::TAOX11_NAMESPACE::Messaging::RelativeRoundtripTimeoutPolicy>::ref_type
    object_traits< ::TAOX11_NAMESPACE::Messaging::RelativeRoundtripTimeoutPolicy>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_MESSAGING::RelativeRoundtripTimeoutPolicy_ptr const polptr =
            TAO_MESSAGING::RelativeRoundtripTimeoutPolicy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<Messaging_impl::RelativeRoundtripTimeoutPolicy> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

    // SyncScopePolicy
    template<>
    TAOX11_MESSAGING_Export object_traits< ::TAOX11_NAMESPACE::Messaging::SyncScopePolicy>::ref_type
    object_traits< ::TAOX11_NAMESPACE::Messaging::SyncScopePolicy>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_MESSAGING::SyncScopePolicy_ptr const polptr =
            TAO_MESSAGING::SyncScopePolicy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<Messaging_impl::SyncScopePolicy> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

    // ConnectionTimeoutPolicy
    template<>
    TAOX11_MESSAGING_Export object_traits< ::TAOX11_NAMESPACE::TAO::ConnectionTimeoutPolicy>::ref_type
    object_traits< ::TAOX11_NAMESPACE::TAO::ConnectionTimeoutPolicy>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_TAO::ConnectionTimeoutPolicy_ptr const polptr =
            TAO_TAO::ConnectionTimeoutPolicy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<Messaging_impl::ConnectionTimeoutPolicy> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

    // BufferingConstraintPolicy
    template<>
    TAOX11_MESSAGING_Export object_traits< ::TAOX11_NAMESPACE::TAO::BufferingConstraintPolicy>::ref_type
    object_traits< ::TAOX11_NAMESPACE::TAO::BufferingConstraintPolicy>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_TAO::BufferingConstraintPolicy_ptr const polptr =
            TAO_TAO::BufferingConstraintPolicy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<Messaging_impl::BufferingConstraintPolicy> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }
  }
} // namespace TAOX11_NAMESPACE
