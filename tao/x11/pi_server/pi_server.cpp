/**
 * @file    pi_server.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 Portable Interceptor Framework
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/pi_server/server_request_interceptor_proxy_factory.h"

#include "tao/x11/pi_server/pi_server.h"

namespace TAOX11_NAMESPACE
{
  int
  PI_Server_Init::Initializer ()
  {
    static int init_result_ =
#if TAO_HAS_INTERCEPTORS == 1
        // load the ServerRequestInterceptor proxy factory
        PortableInterceptor_impl::ServerRequestInterceptorProxyFactory::Initializer ();
#else
        0;
#endif  /* TAO_HAS_INTERCEPTORS */
    return init_result_;
  }
}
