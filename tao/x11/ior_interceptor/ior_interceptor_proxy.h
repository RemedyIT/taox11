/**
 * @file    ior_interceptor_proxy.h
 * @author  Martin Corino
 *
 * @brief   TAO PI IORInterceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_IORINTERCEPTOR_PROXY_H_INCLUDED
#define TAOX11_PI_IORINTERCEPTOR_PROXY_H_INCLUDED

#include "tao/x11/ior_interceptor/taox11_ior_interceptor_export.h"

// TAO dependencies
#include "tao/x11/ior_interceptor/tao_ior_interceptor.h"

// TAOX11 dependencies
#include "tao/x11/pi/interceptor_proxy.h"

#include "tao/x11/ior_interceptor/ior_interceptor.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class IORInterceptor_proxy final
      : public virtual PortableInterceptor_impl::Interceptor_proxy,
        public virtual TAO_PI::IORInterceptor_3_0
    {
    public:
      explicit IORInterceptor_proxy (
          IDL::traits<PortableInterceptor::IORInterceptor>::ref_type x11_interceptor);

      virtual
      void
      establish_components (
          TAO_PI::IORInfo_ptr info) override;

      virtual
      void
      components_established (
          TAO_PI::IORInfo_ptr info) override;

      virtual
      void
      adapter_manager_state_changed (
          const char * id,
          TAO_PI::AdapterState state) override;

      virtual
      void
      adapter_state_changed (
          const TAO_PI::ObjectReferenceTemplateSeq& templates,
          TAO_PI::AdapterState state) override;

    private:
      IDL::traits<PortableInterceptor::IORInterceptor>::ref_type x11_interceptor_;

      IORInterceptor_proxy () = delete;
      IORInterceptor_proxy (const IORInterceptor_proxy&) = delete;
      IORInterceptor_proxy (IORInterceptor_proxy&&) = delete;
      IORInterceptor_proxy& operator =(const IORInterceptor_proxy&) = delete;
      IORInterceptor_proxy& operator =(IORInterceptor_proxy&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#endif /* TAOX11_PI_IORINTERCEPTOR_PROXY_H_INCLUDED */
