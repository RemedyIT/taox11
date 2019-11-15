/**
 * @file    client_orbinitializer.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the PortableInterceptor::ForwardRequest
 *          exception test client side ORB initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "client_orbinitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "client_request_interceptor.h"

Client_ORBInitializer::Client_ORBInitializer (const std::string& forward_str)
  : forward_str_ (forward_str)
{}

void
Client_ORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type)
{
}

void
Client_ORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  std::string const orb_id = info->orb_id ();

  IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type client_interceptor =
    CORBA::make_reference<Client_Request_Interceptor> (orb_id, this->forward_str_);

  info->add_client_request_interceptor (client_interceptor);
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
