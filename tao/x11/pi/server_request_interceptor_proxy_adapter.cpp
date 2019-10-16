/**
 * @file    server_request_interceptor_proxy_adapter.cpp
 * @author  Martin Corino
 *
 * @brief   ServerRequestInterceptor_proxy factory adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/pi/server_request_interceptor_proxy_adapter.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    std::string ServerRequestInterceptorProxyAdapter::concrete_adapter_name_;
  }
} // namespace TAOX11_NAMESPACE

#endif
