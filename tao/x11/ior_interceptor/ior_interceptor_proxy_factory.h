/**
 * @file    ior_interceptor_proxy_factory.h
 * @author  Martin Corino
 *
 * @brief   IORInterceptor_proxy factory implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_IORINTERCEPTOR_PROXY_FACTORY_INCLUDED
#define TAOX11_IORINTERCEPTOR_PROXY_FACTORY_INCLUDED

#pragma once

#include "ace/Service_Config.h"
#include "tao/x11/ior_interceptor/taox11_ior_interceptor_export.h"
#include "tao/x11/pi/ior_interceptor_proxy_adapter.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_IOR_Interceptor_Export IORInterceptorProxyFactory final
      : public IORInterceptorProxyAdapter
    {
    public:
      ~IORInterceptorProxyFactory() = default;

      TAO_PI::IORInterceptor_ptr
      create_ior_interceptor_proxy (
          IDL::traits<PortableInterceptor::IORInterceptor>::ref_type) override;

      void
      release_ior_interceptor_proxy (
          TAO_PI::IORInterceptor_ptr) override;

      static int Initializer ();
    };

    ACE_STATIC_SVC_DECLARE (IORInterceptorProxyFactory)
    ACE_FACTORY_DECLARE (TAOX11_IOR_Interceptor, IORInterceptorProxyFactory)
  }
} // namespace TAOX11_NAMESPACE

#endif

#endif /* TAOX11_SERVERREQUESTINTERCEPTOR_PROXY_FACTORY_INCLUDED */
