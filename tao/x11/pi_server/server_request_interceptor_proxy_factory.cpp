/**
 * @file    server_request_interceptor_proxy_factory.cpp
 * @author  Martin Corino
 *
 * @brief   ServerRequestInterceptor_proxy factory implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// pull in TAO dependencies first
#include "tao/x11/tao_corba.h"
#include "tao/x11/pi_server/tao_pi_server.h"

// TAOX11 dependencies
#include "tao/x11/pi_server/server_request_interceptor_proxy_factory.h"

#include "tao/x11/pi_server/server_request_interceptor_proxy.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/log.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    TAO_PI::ServerRequestInterceptor_ptr
    ServerRequestInterceptorProxyFactory::create_server_request_interceptor_proxy (
        IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type interceptor)
    {
      TAO_PI::ServerRequestInterceptor_ptr tao_sri;
      ACE_NEW_THROW_EX (tao_sri,
                        PortableInterceptor_impl::ServerRequestInterceptor_proxy (
                            interceptor),
                        CORBA::NO_MEMORY (
                          TAO_CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      return tao_sri;
    }

    void
    ServerRequestInterceptorProxyFactory::release_server_request_interceptor_proxy (
        TAO_PI::ServerRequestInterceptor_ptr interceptor)
    {
      TAO_CORBA::release (interceptor);
    }

    int
    ServerRequestInterceptorProxyFactory::Initializer ()
    {
      ServerRequestInterceptorProxyAdapter::set_concrete_adapter_name ("X11_ServerRequestInterceptorProxyFactory");

      return
        ACE_Service_Config::process_directive (
            ace_svc_desc_ServerRequestInterceptorProxyFactory);
    }

    ACE_STATIC_SVC_DEFINE (
        ServerRequestInterceptorProxyFactory,
        ACE_TEXT ("X11_ServerRequestInterceptorProxyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServerRequestInterceptorProxyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAOX11_PI_Server, ServerRequestInterceptorProxyFactory)
  }
} // namespace TAOX11_NAMESPACE

#endif
