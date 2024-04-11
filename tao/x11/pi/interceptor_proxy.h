/**
 * @file    interceptor_proxy.h
 * @author  Martin Corino
 *
 * @brief   TAO PI Interceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_INTERCEPTOR_PROXY_H_INCLUDED
#define TAOX11_PI_INTERCEPTOR_PROXY_H_INCLUDED

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/base/tao_corba.h"
#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/pi/pi.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PortableInterceptor_Export Interceptor_proxy
      : public virtual TAO_PI::Interceptor,
        public virtual TAO_CORBA::LocalObject
    {
    public:
      explicit Interceptor_proxy (
          IDL::traits<PortableInterceptor::Interceptor>::ref_type x11_interceptor);

      char * name () override;

      void destroy () override;

    private:
      IDL::traits<PortableInterceptor::Interceptor>::ref_type x11_interceptor_;

      Interceptor_proxy () = delete;
      Interceptor_proxy (const Interceptor_proxy&) = delete;
      Interceptor_proxy (Interceptor_proxy&&) = delete;
      Interceptor_proxy& operator =(const Interceptor_proxy&) = delete;
      Interceptor_proxy& operator =(Interceptor_proxy&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_INTERCEPTOR_PROXY_H_INCLUDED */
