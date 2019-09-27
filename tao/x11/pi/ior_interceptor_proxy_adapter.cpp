/**
 * @file    ior_interceptor_proxy_adapter.cpp
 * @author  Martin Corino
 *
 * @brief   IORInterceptor_proxy factory adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/pi/ior_interceptor_proxy_adapter.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    std::string IORInterceptorProxyAdapter::concrete_adapter_name_;
  }
} // namespace TAOX11_NAMESPACE

#endif
