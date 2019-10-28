/**
 * @file    server_request_interceptor_proxy.h
 * @author  Martin Corino
 *
 * @brief   TAO PI ServerRequestInterceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_SERVERREQUESTINTERCEPTOR_PROXY_H_INCLUDED
#define TAOX11_PI_SERVERREQUESTINTERCEPTOR_PROXY_H_INCLUDED

#include "tao/x11/pi_server/tao_pi_server.h"

#include "tao/x11/pi/interceptor_proxy.h"

#include "tao/x11/pi_server/pi_server.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class ServerRequestInterceptor_proxy final
      : public virtual PortableInterceptor_impl::Interceptor_proxy,
        public virtual TAO_PI::ServerRequestInterceptor
    {
    public:
      explicit ServerRequestInterceptor_proxy (
          IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type x11_interceptor);

      virtual
      void
      receive_request_service_contexts (
          TAO_PI::ServerRequestInfo_ptr ri) override;

      virtual
      void
      receive_request (
          TAO_PI::ServerRequestInfo_ptr ri) override;

      virtual
      void
      send_reply (
          TAO_PI::ServerRequestInfo_ptr ri) override;

      virtual
      void
      send_exception (
          TAO_PI::ServerRequestInfo_ptr ri) override;

      virtual
      void
      send_other (
          TAO_PI::ServerRequestInfo_ptr ri) override;

    private:
      IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type x11_interceptor_;

      ServerRequestInterceptor_proxy () = delete;
      ServerRequestInterceptor_proxy (const ServerRequestInterceptor_proxy&) = delete;
      ServerRequestInterceptor_proxy (ServerRequestInterceptor_proxy&&) = delete;
      ServerRequestInterceptor_proxy& operator =(const ServerRequestInterceptor_proxy&) = delete;
      ServerRequestInterceptor_proxy& operator =(ServerRequestInterceptor_proxy&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#endif /* TAOX11_PI_SERVERREQUESTINTERCEPTOR_PROXY_H_INCLUDED */
