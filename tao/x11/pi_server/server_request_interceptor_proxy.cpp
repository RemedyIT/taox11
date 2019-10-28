/**
 * @file    server_request_interceptor_proxy.cpp
 * @author  Martin Corino
 *
 * @brief   TAO PI ServerRequestInterceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/pi_server/server_request_interceptor_proxy.h"

#include "tao/x11/pi_server/server_request_info.h"

#include "tao/x11/user_exception_proxy_out.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    ServerRequestInterceptor_proxy::ServerRequestInterceptor_proxy (
        IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type x11_interceptor)
      : PortableInterceptor_impl::Interceptor_proxy (x11_interceptor),
        TAO_PI::ServerRequestInterceptor ()
    {
      // making sure to move AFTER ctor Interceptor_proxy
      this->x11_interceptor_ = std::move (x11_interceptor);
    }

    void
    ServerRequestInterceptor_proxy::receive_request_service_contexts (
        TAO_PI::ServerRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ServerRequestInfo> (
                TAO_PI::ServerRequestInfo::_duplicate(ri));
        this->x11_interceptor_->receive_request_service_contexts (x11_ri);
      }
      catch (const PortableInterceptor::ForwardRequest& ex)
      {
        throw TAO_PI::ForwardRequest (ex.forward ()->get_proxy ()->get ());
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }

    void
    ServerRequestInterceptor_proxy::receive_request (
        TAO_PI::ServerRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ServerRequestInfo> (
                TAO_PI::ServerRequestInfo::_duplicate(ri));
        this->x11_interceptor_->receive_request (x11_ri);
      }
      catch (const PortableInterceptor::ForwardRequest& ex)
      {
        throw TAO_PI::ForwardRequest (ex.forward ()->get_proxy ()->get ());
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }

    void
    ServerRequestInterceptor_proxy::send_reply (
        TAO_PI::ServerRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ServerRequestInfo> (
                TAO_PI::ServerRequestInfo::_duplicate(ri));
        this->x11_interceptor_->send_reply (x11_ri);
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }

    void
    ServerRequestInterceptor_proxy::send_exception (
        TAO_PI::ServerRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ServerRequestInfo> (
                TAO_PI::ServerRequestInfo::_duplicate(ri));
        this->x11_interceptor_->send_exception (x11_ri);
      }
      catch (const PortableInterceptor::ForwardRequest& ex)
      {
        throw TAO_PI::ForwardRequest (ex.forward ()->get_proxy ()->get ());
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }

    void
    ServerRequestInterceptor_proxy::send_other (
        TAO_PI::ServerRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ServerRequestInfo> (
                TAO_PI::ServerRequestInfo::_duplicate(ri));
        this->x11_interceptor_->send_other (x11_ri);
      }
      catch (const PortableInterceptor::ForwardRequest& ex)
      {
        throw TAO_PI::ForwardRequest (ex.forward ()->get_proxy ()->get ());
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }
  } // namespace PortableInterceptor_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAO_HAS_INTERCEPTORS == 1 */
