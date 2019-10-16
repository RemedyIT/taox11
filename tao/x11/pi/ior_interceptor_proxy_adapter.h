/**
 * @file    ior_interceptor_proxy_adapter.h
 * @author  Martin Corino
 *
 * @brief   IORInterceptor_proxy factory adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_IORINTERCEPTOR_PROXY_ADAPTER_INCLUDED
#define TAOX11_IORINTERCEPTOR_PROXY_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/pi/pi.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PortableInterceptor_Export IORInterceptorProxyAdapter
      : public ACE_Service_Object
    {
    public:
      ~IORInterceptorProxyAdapter () = default;

      virtual
      TAO_PI::IORInterceptor_ptr
      create_ior_interceptor_proxy (
          IDL::traits<PortableInterceptor::IORInterceptor>::ref_type) = 0;

      virtual
      void
      release_ior_interceptor_proxy (
          TAO_PI::IORInterceptor_ptr) = 0;

      static void set_concrete_adapter_name (std::string name)
      {
        concrete_adapter_name_ = std::move (name);
      }

      static const std::string& concrete_adapter_name ()
      {
        return concrete_adapter_name_;
      }

    private:
      static std::string concrete_adapter_name_;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#endif /* TAOX11_IORINTERCEPTOR_PROXY_ADAPTER_INCLUDED */
