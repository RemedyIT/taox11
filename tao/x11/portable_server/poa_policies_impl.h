/**
 * @file    poa_policies_impl.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA Policy implementation classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_POA_POLICIES_IMPL_H_INCLUDED
#define TAOX11_POA_POLICIES_IMPL_H_INCLUDED

#include "tao/x11/portable_server/portableserver.h"

namespace TAOX11_NAMESPACE {

  namespace PortableServer
  {

#define TAOX11_POA_POLICY_DECL(name) \
    class TAOX11_PortableServer_Export name ## _impl final \
      : public virtual TAOX11_NAMESPACE::PortableServer::name \
    { \
    public: \
      friend struct CORBA::object_traits< name>; \
    \
      TAOX11_NAMESPACE::PortableServer::name ## Value value() override; \
    \
    protected: \
      template <typename _Tp1, typename, typename ...Args> \
      friend constexpr CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args); \
      \
      explicit name ## _impl (TAO_PORTABLE_SERVER::name ## _ptr _pol); \
      ~name ## _impl () = default; \
      CORBA::object_reference<TAOX11_CORBA::Policy> copy_i () override; \
    \
    private: \
      TAO_PORTABLE_SERVER::name ## _ptr pol_ {}; \
      name ## _impl () = delete; \
      name ## _impl (const name ## _impl&) = delete; \
      name ## _impl (name ## _impl&&) = delete; \
      name ## _impl& operator= (const name ## _impl&) = delete; \
      name ## _impl& operator= (name ## _impl&&) = delete; \
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    TAOX11_POA_POLICY_DECL (ThreadPolicy);
#endif
#if ! defined (CORBA_E_MICRO)
    TAOX11_POA_POLICY_DECL (LifespanPolicy);
    TAOX11_POA_POLICY_DECL (IdUniquenessPolicy);
    TAOX11_POA_POLICY_DECL (IdAssignmentPolicy);
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    TAOX11_POA_POLICY_DECL (ImplicitActivationPolicy);
    TAOX11_POA_POLICY_DECL (ServantRetentionPolicy);
    TAOX11_POA_POLICY_DECL (RequestProcessingPolicy);
#endif

  } // namespacePortableServer
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_POA_POLICIES_IMPL_H_INCLUDED
