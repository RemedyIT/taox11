/**
 * @file    poa_policies_impl.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA Policy implementation classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/objproxy.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/servantbase.h"

#include "tao/x11/portable_server/poa_policies_impl.h"

namespace TAOX11_NAMESPACE {

  namespace PortableServer
  {

#define TAOX11_POA_POLICY_IMPL(name) \
    TAOX11_NAMESPACE::PortableServer::name ## Value name ## _impl::value() \
    { \
      return static_cast <TAOX11_NAMESPACE::PortableServer::name ## Value> (this->pol_->value ()); \
    } \
    \
    name ## _impl::name ## _impl (TAO_PORTABLE_SERVER::name ## _ptr _pol) \
      : name (), \
        pol_ (_pol) \
    { \
      this->set_proxy (new TAOX11_NAMESPACE::Object_proxy (_pol)); \
    } \
    \
    IDL::traits<TAOX11_CORBA::Policy>::ref_type name ## _impl::copy_i () \
    { \
      try { \
        TAO_CORBA::Policy_var _pp = TAO_CORBA::Policy::_narrow (this->get_proxy ()->get ()); \
        TAO_CORBA::Policy_var _pp_copy = _pp->copy (); \
        TAO_PORTABLE_SERVER::name ## _ptr _pp_i = \
          TAO_PORTABLE_SERVER::name::_narrow (_pp_copy); \
        if (!TAO_CORBA::is_nil (_pp_i)) \
        { \
          return CORBA::make_reference<name ## _impl> (_pp_i); \
        } \
      } \
      catch_tao_system_ex(_ex) \
      return {}; \
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    TAOX11_POA_POLICY_IMPL (ThreadPolicy)
#endif
#if ! defined (CORBA_E_MICRO)
    TAOX11_POA_POLICY_IMPL (LifespanPolicy)
    TAOX11_POA_POLICY_IMPL (IdUniquenessPolicy)
    TAOX11_POA_POLICY_IMPL (IdAssignmentPolicy)
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    TAOX11_POA_POLICY_IMPL (ImplicitActivationPolicy)
    TAOX11_POA_POLICY_IMPL (ServantRetentionPolicy)
    TAOX11_POA_POLICY_IMPL (RequestProcessingPolicy)
#endif

  } // namespace PortableServer

  namespace CORBA
  {
#undef TAOX11_POA_POLICY_IMPL
#define TAOX11_POA_POLICY_IMPL(name) \
    template <> \
    CORBA::object_traits< PortableServer::name>::ref_type \
    CORBA::object_traits< PortableServer::name>::narrow(IDL::traits<TAOX11_CORBA::Object>::ref_type obj) \
    { \
      if (obj) \
      { \
        if (obj->_is_local ()) \
        { \
          return ref_type::_narrow (std::move(obj)); \
        } \
        try { \
          TAO_PORTABLE_SERVER::name ## _ptr const _pp = \
            TAO_PORTABLE_SERVER::name::_narrow (obj->get_proxy ()->get ()); \
          if (!TAO_CORBA::is_nil (_pp)) \
          { \
            return CORBA::object_traits< PortableServer::name>::ref_type (new PortableServer::name ## _impl (_pp)); \
          } \
        } \
        catch_tao_system_ex(_ex) \
      } \
      return {}; \
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    TAOX11_POA_POLICY_IMPL (ThreadPolicy)
#endif
#if ! defined (CORBA_E_MICRO)
    TAOX11_POA_POLICY_IMPL (LifespanPolicy)
    TAOX11_POA_POLICY_IMPL (IdUniquenessPolicy)
    TAOX11_POA_POLICY_IMPL (IdAssignmentPolicy)
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    TAOX11_POA_POLICY_IMPL (ImplicitActivationPolicy)
    TAOX11_POA_POLICY_IMPL (ServantRetentionPolicy)
    TAOX11_POA_POLICY_IMPL (RequestProcessingPolicy)
#endif
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
