/**
 * @file    ior_interceptor_proxy_factory.cpp
 * @author  Martin Corino
 *
 * @brief   IORInterceptor_proxy factory implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// pull in TAO dependencies first
#include "tao/x11/tao_corba.h"
#include "tao/x11/ior_interceptor/tao_ior_interceptor.h"

// TAOX11 dependencies
#include "tao/x11/ior_interceptor/ior_interceptor_proxy_factory.h"

#include "tao/x11/ior_interceptor/ior_interceptor_proxy.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/log.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    TAO_PI::IORInterceptor_ptr
    IORInterceptorProxyFactory::create_ior_interceptor_proxy (
        IDL::traits<PortableInterceptor::IORInterceptor>::ref_type interceptor)
    {
      TAO_PI::IORInterceptor_ptr tao_ii;
      ACE_NEW_THROW_EX (tao_ii,
                        PortableInterceptor_impl::IORInterceptor_proxy (
                            interceptor),
                        CORBA::NO_MEMORY (
                          TAO_CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      return tao_ii;
    }

    void
    IORInterceptorProxyFactory::release_ior_interceptor_proxy (
        TAO_PI::IORInterceptor_ptr interceptor)
    {
      TAO_CORBA::release (interceptor);
    }

    int
    IORInterceptorProxyFactory::Initializer ()
    {
      IORInterceptorProxyAdapter::set_concrete_adapter_name ("X11_IORInterceptorProxyFactory");

      return
        ACE_Service_Config::process_directive (
            ace_svc_desc_IORInterceptorProxyFactory);
    }

    ACE_STATIC_SVC_DEFINE (
        IORInterceptorProxyFactory,
        ACE_TEXT ("X11_IORInterceptorProxyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IORInterceptorProxyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAOX11_IOR_Interceptor, IORInterceptorProxyFactory)
  }
} // namespace TAOX11_NAMESPACE

#endif
