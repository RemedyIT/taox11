/**
 * @file    interceptor_proxy.cpp
 * @author  Martin Corino
 *
 * @brief   TAO PI Interceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/pi/interceptor_proxy.h"

#include "tao/x11/user_exception_proxy_out.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    Interceptor_proxy::Interceptor_proxy (
        IDL::traits<PortableInterceptor::Interceptor>::ref_type x11_interceptor)
      : TAO_PI::Interceptor (),
        x11_interceptor_ (std::move (x11_interceptor))
    {}

    char *
    Interceptor_proxy::name ()
    {
      return TAO_CORBA::string_dup (this->x11_interceptor_->name ().c_str ());
    }

    void
    Interceptor_proxy::destroy ()
    {
      try
      {
        this->x11_interceptor_->destroy ();
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
