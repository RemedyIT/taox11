/**
 * @file    client_request_interceptor_proxy.h
 * @author  Martin Corino
 *
 * @brief   TAO PI ClientRequestInterceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_PI_CLIENTREQUESTINTERCEPTOR_PROXY_H_INCLUDED
#define TAOX11_PI_CLIENTREQUESTINTERCEPTOR_PROXY_H_INCLUDED

#include "tao/x11/pi/interceptor_proxy.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class ClientRequestInterceptor_proxy final
      : public virtual PortableInterceptor_impl::Interceptor_proxy,
        public virtual TAO_PI::ClientRequestInterceptor
    {
    public:
      explicit ClientRequestInterceptor_proxy (
          IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type x11_interceptor);

      virtual
      void
      send_request (
          TAO_PI::ClientRequestInfo_ptr ri) override;

      virtual
      void
      send_poll (
          TAO_PI::ClientRequestInfo_ptr ri) override;

      virtual
      void
      receive_reply (
          TAO_PI::ClientRequestInfo_ptr ri) override;

      virtual
      void
      receive_exception (
          TAO_PI::ClientRequestInfo_ptr ri) override;

      virtual
      void
      receive_other (
          TAO_PI::ClientRequestInfo_ptr ri) override;

    private:
      IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type x11_interceptor_;

      ClientRequestInterceptor_proxy () = delete;
      ClientRequestInterceptor_proxy (const ClientRequestInterceptor_proxy&) = delete;
      ClientRequestInterceptor_proxy (ClientRequestInterceptor_proxy&&) = delete;
      ClientRequestInterceptor_proxy& operator =(const ClientRequestInterceptor_proxy&) = delete;
      ClientRequestInterceptor_proxy& operator =(ClientRequestInterceptor_proxy&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_CLIENTREQUESTINTERCEPTOR_PROXY_H_INCLUDED */
