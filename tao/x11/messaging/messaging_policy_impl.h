/**
 * @file    messaging_policy_impl.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Messaging library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_MESSAGING_POLICY_IMPL_H
#define TAOX11_MESSAGING_POLICY_IMPL_H

#include "tao/x11/messaging/taox11_messaging_export.h"
#include "tao/x11/messaging/tao_messaging.h"

#include "tao/x11/objproxy.h"

#include "tao/x11/messaging/Messaging_RT_PolicyC.h"
#include "tao/x11/messaging/Messaging_SyncScope_PolicyC.h"
#include "tao/x11/messaging/TAO_ExtC.h"


namespace TAOX11_NAMESPACE
{
  namespace Messaging_impl
  {
    /*--------------------------------------------------------------------
     *
     * RelativeRoundtripTimeoutPolicy
     *
    --------------------------------------------------------------------*/
    class TAOX11_MESSAGING_Export RelativeRoundtripTimeoutPolicy final
      : public Messaging::RelativeRoundtripTimeoutPolicy
    {
    public:
      RelativeRoundtripTimeoutPolicy (TAO_MESSAGING::RelativeRoundtripTimeoutPolicy_ptr polptr);

      ~RelativeRoundtripTimeoutPolicy () = default;

      ::TAOX11_NAMESPACE::TimeBase::TimeT relative_expiry() override;

    private:
      TAO_MESSAGING::RelativeRoundtripTimeoutPolicy_ptr pol_;

      RelativeRoundtripTimeoutPolicy () = delete;
      RelativeRoundtripTimeoutPolicy (const RelativeRoundtripTimeoutPolicy&) = delete;
      RelativeRoundtripTimeoutPolicy (RelativeRoundtripTimeoutPolicy&&) = delete;
      RelativeRoundtripTimeoutPolicy& operator= (const RelativeRoundtripTimeoutPolicy&) = delete;
      RelativeRoundtripTimeoutPolicy& operator= (RelativeRoundtripTimeoutPolicy&&) = delete;
    };

    /*--------------------------------------------------------------------
     *
     * SyncScopePolicy
     *
    --------------------------------------------------------------------*/
    class TAOX11_MESSAGING_Export SyncScopePolicy final
      : public Messaging::SyncScopePolicy
    {
    public:
      SyncScopePolicy (TAO_MESSAGING::SyncScopePolicy_ptr polptr);

      ~SyncScopePolicy () = default;

      ::TAOX11_NAMESPACE::Messaging::SyncScope synchronization () override;

    private:
      TAO_MESSAGING::SyncScopePolicy_ptr pol_;

      SyncScopePolicy () = delete;
      SyncScopePolicy (const SyncScopePolicy&) = delete;
      SyncScopePolicy (SyncScopePolicy&&) = delete;
      SyncScopePolicy& operator= (const SyncScopePolicy&) = delete;
      SyncScopePolicy& operator= (SyncScopePolicy&&) = delete;
    };

    /*--------------------------------------------------------------------
     *
     * ConnectionTimeoutPolicy
     *
    --------------------------------------------------------------------*/
    class TAOX11_MESSAGING_Export ConnectionTimeoutPolicy final
      : public TAO::ConnectionTimeoutPolicy
    {
    public:
      ConnectionTimeoutPolicy (TAO_TAO::ConnectionTimeoutPolicy_ptr polptr);

      ~ConnectionTimeoutPolicy () = default;

      ::TAOX11_NAMESPACE::TimeBase::TimeT relative_expiry() override;

    private:
      TAO_TAO::ConnectionTimeoutPolicy_ptr pol_;

      ConnectionTimeoutPolicy () = delete;
      ConnectionTimeoutPolicy (const ConnectionTimeoutPolicy&) = delete;
      ConnectionTimeoutPolicy (ConnectionTimeoutPolicy&&) = delete;
      ConnectionTimeoutPolicy& operator= (const ConnectionTimeoutPolicy&) = delete;
      ConnectionTimeoutPolicy& operator= (ConnectionTimeoutPolicy&&) = delete;
    };

    /*--------------------------------------------------------------------
     *
     * BufferingConstraintPolicy
     *
    --------------------------------------------------------------------*/
    class TAOX11_MESSAGING_Export BufferingConstraintPolicy final
      : public TAO::BufferingConstraintPolicy
    {
    public:
      BufferingConstraintPolicy (TAO_TAO::BufferingConstraintPolicy_ptr polptr);

      ~BufferingConstraintPolicy () = default;

      TAO::BufferingConstraint buffering_constraint() override;

    private:
      TAO_TAO::BufferingConstraintPolicy_ptr pol_;

      BufferingConstraintPolicy () = delete;
      BufferingConstraintPolicy (const BufferingConstraintPolicy&) = delete;
      BufferingConstraintPolicy (BufferingConstraintPolicy&&) = delete;
      BufferingConstraintPolicy& operator= (const BufferingConstraintPolicy&) = delete;
      BufferingConstraintPolicy& operator= (BufferingConstraintPolicy&&) = delete;
    };

  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_MESSAGING_POLICY_IMPL_H */
