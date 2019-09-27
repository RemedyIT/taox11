/**
 * @file    server_orbinitializer.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the PortableInterceptor::ForwardRequest
 *          exception test server side ORB initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "server_orbinitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "server_request_interceptor.h"

#include "testlib/taox11_testlog.h"

Server_ORBInitializer::Server_ORBInitializer ()
  : server_interceptor_ ()
{
}

void
Server_ORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type)
{
}

void
Server_ORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  this->server_interceptor_ =
      CORBA::make_reference<Server_Request_Interceptor> ();

  // Install the server request interceptor.
  info->add_server_request_interceptor (this->server_interceptor_);
}

IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type
Server_ORBInitializer::server_interceptor ()
{
  return this->server_interceptor_;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
