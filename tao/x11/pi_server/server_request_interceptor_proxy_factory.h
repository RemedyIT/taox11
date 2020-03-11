/**
 * @file    server_request_interceptor_proxy_factory.h
 * @author  Martin Corino
 *
 * @brief   ServerRequestInterceptor_proxy factory implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_SERVERREQUESTINTERCEPTOR_PROXY_FACTORY_INCLUDED
#define TAOX11_SERVERREQUESTINTERCEPTOR_PROXY_FACTORY_INCLUDED

#pragma once

#include "ace/Service_Config.h"
#include "tao/x11/pi_server/taox11_pi_server_export.h"
#include "tao/x11/pi/server_request_interceptor_proxy_adapter.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PI_Server_Export ServerRequestInterceptorProxyFactory final
      : public ServerRequestInterceptorProxyAdapter
    {
    public:
      ~ServerRequestInterceptorProxyFactory() = default;

      TAO_PI::ServerRequestInterceptor_ptr
      create_server_request_interceptor_proxy (
          IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type) override;

      void
      release_server_request_interceptor_proxy (
          TAO_PI::ServerRequestInterceptor_ptr) override;

      static int Initializer ();
    };

    ACE_STATIC_SVC_DECLARE (ServerRequestInterceptorProxyFactory)
    ACE_FACTORY_DECLARE (TAOX11_PI_Server, ServerRequestInterceptorProxyFactory)
  }
} // namespace TAOX11_NAMESPACE

#endif

#endif /* TAOX11_SERVERREQUESTINTERCEPTOR_PROXY_FACTORY_INCLUDED */
