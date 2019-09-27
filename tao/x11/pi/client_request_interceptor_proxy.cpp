/**
 * @file    client_request_interceptor_proxy.cpp
 * @author  Martin Corino
 *
 * @brief   TAO PI ClientRequestInterceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/pi/client_request_interceptor_proxy.h"

#include "tao/x11/pi/client_request_info.h"

#include "tao/x11/user_exception_proxy_out.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    ClientRequestInterceptor_proxy::ClientRequestInterceptor_proxy (
        IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type x11_interceptor)
      : PortableInterceptor_impl::Interceptor_proxy (x11_interceptor),
        TAO_PI::ClientRequestInterceptor ()
    {
      // making sure to move AFTER ctor Interceptor_proxy
      this->x11_interceptor_ = std::move (x11_interceptor);
    }

    void
    ClientRequestInterceptor_proxy::send_request (
        TAO_PI::ClientRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ClientRequestInfo> (
                TAO_PI::ClientRequestInfo::_duplicate(ri));
        this->x11_interceptor_->send_request (x11_ri);
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
    ClientRequestInterceptor_proxy::send_poll (
        TAO_PI::ClientRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ClientRequestInfo> (
                TAO_PI::ClientRequestInfo::_duplicate(ri));
        this->x11_interceptor_->send_poll (x11_ri);
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
    ClientRequestInterceptor_proxy::receive_reply (
        TAO_PI::ClientRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ClientRequestInfo> (
                TAO_PI::ClientRequestInfo::_duplicate(ri));
        this->x11_interceptor_->receive_reply (x11_ri);
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
    ClientRequestInterceptor_proxy::receive_exception (
        TAO_PI::ClientRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ClientRequestInfo> (
                TAO_PI::ClientRequestInfo::_duplicate(ri));
        this->x11_interceptor_->receive_exception (x11_ri);
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
    ClientRequestInterceptor_proxy::receive_other (
        TAO_PI::ClientRequestInfo_ptr ri)
    {
      try
      {
        IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type x11_ri =
            CORBA::make_reference<PortableInterceptor_impl::ClientRequestInfo> (
                TAO_PI::ClientRequestInfo::_duplicate(ri));
        this->x11_interceptor_->receive_other (x11_ri);
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
  }
} // namespace TAOX11_NAMESPACE
