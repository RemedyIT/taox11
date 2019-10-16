/**
 * @file    ior_interceptor.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 IOR Interceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/ior_interceptor/ior_interceptor_proxy_factory.h"

#include "tao/x11/ior_interceptor/ior_interceptor.h"

namespace TAOX11_NAMESPACE
{
  int
  IOR_Interceptor_Init::Initializer ()
  {
    static int init_result_ =
#if TAO_HAS_INTERCEPTORS == 1
        // load the ServerRequestInterceptor proxy factory
        PortableInterceptor_impl::IORInterceptorProxyFactory::Initializer ();
#else
        0;
#endif  /* TAO_HAS_INTERCEPTORS */
    return init_result_;
  }
}
