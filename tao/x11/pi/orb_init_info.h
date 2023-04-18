/**
 * @file    orb_init_info.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI ORBInitInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_ORBINITINFO_H_INCLUDED
#define TAOX11_PI_ORBINITINFO_H_INCLUDED

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/pi/PI_includeC.h"
#include "tao/x11/codecfactory/codecfactory.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/x11/pi/server_request_interceptor_proxy_adapter.h"
# include "tao/x11/pi/ior_interceptor_proxy_adapter.h"
#endif

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PortableInterceptor_Export ORBInitInfo final
      : public IDL::traits<PortableInterceptor::ORBInitInfo_3_1>::base_type
    {
    public:
      explicit ORBInitInfo (TAO_PI::ORBInitInfo_3_1_ptr orbii);

      virtual ~ORBInitInfo () = default;

      ::TAOX11_NAMESPACE::CORBA::StringSeq arguments () override;

      std::string orb_id () override;

      IDL::traits<IOP::CodecFactory>::ref_type codec_factory () override;

      void
      register_initial_reference (
          const ObjectId& id,
          IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type obj) override;

      IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type
      resolve_initial_references (
          const ObjectId& id) override;

      void
      add_client_request_interceptor (
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ClientRequestInterceptor>::ref_type interceptor) override;

      void
      add_server_request_interceptor (
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ServerRequestInterceptor>::ref_type interceptor) override;

      void
      add_ior_interceptor (
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::IORInterceptor>::ref_type interceptor) override;

      ::TAOX11_NAMESPACE::PortableInterceptor::SlotId allocate_slot_id () override;

      void
      register_policy_factory (
          ::TAOX11_NAMESPACE::CORBA::PolicyType type,
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::PolicyFactory>::ref_type policy_factory) override;

      void
      add_client_request_interceptor_with_policy (
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ClientRequestInterceptor>::ref_type interceptor,
          const CORBA::PolicyList& policies) override;

      void
      add_server_request_interceptor_with_policy (
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ServerRequestInterceptor>::ref_type interceptor,
          const CORBA::PolicyList& policies) override;

      void
      add_ior_interceptor_with_policy (
          IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::IORInterceptor>::ref_type interceptor,
          const CORBA::PolicyList& policies) override;


      IDL::traits<CORBA::ORB>::ref_type _get_orb () override;

    private:
#if TAO_HAS_INTERCEPTORS == 1

      friend struct sri_proxy_guard;
      friend struct iori_proxy_guard;

      static ServerRequestInterceptorProxyAdapter* get_sri_proxy_adapter ();
      static ServerRequestInterceptorProxyAdapter* load_sri_proxy_adapter ();

      static IORInterceptorProxyAdapter* get_iorinterceptor_proxy_adapter ();
      static IORInterceptorProxyAdapter* load_iorinterceptor_proxy_adapter ();

#endif

      TAO_PI::ORBInitInfo_3_1_ptr orbii_;

      ORBInitInfo () = delete;
      ORBInitInfo (const ORBInitInfo&) = delete;
      ORBInitInfo (ORBInitInfo&&) = delete;
      ORBInitInfo& operator= (const ORBInitInfo&) = delete;
      ORBInitInfo& operator= (ORBInitInfo&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_ORBINITINFO_H_INCLUDED */
